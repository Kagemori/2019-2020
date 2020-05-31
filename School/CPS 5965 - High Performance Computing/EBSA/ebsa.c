#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#define MAX_THREADS 8

double funcN(int n, double *x_arr, void * v_opts) {
    double r=0;
    int i;
    double retval;
    for(i=0;i<n;i++) {
        r += x_arr[i]*x_arr[i];
    }
    r = sqrt(r);
    retval=1;
    for(i=0;i<n;i++) {
        retval *= sin(x_arr[i]);
    }
    retval *= exp(-r*r);
    retval += 1.0e-20*r*r;
    return retval;
}

double func(int n, double * x_arr, void * v_opts) {
     // sample function
     double x = x_arr[0];
     double y;
     y = x_arr[1];
     double f = -sin(x)*sin(y)*exp(-(x*x+y*y))+1.0e-20*(x*x+y*y);
     //double f = x*x+y*y;
     return f;
}

void print_array(int n, double * x) {
    int i;
    for(i=0;i<n;i++) {
        printf("%lf ",x[i]);
    }
    printf("\n");
}


typedef struct {
    double energy;
    double temp;
    double step;
    double target;
    double allowance;
    double step_factor;
    double cooling_factor;
    double epsilon;
    int n;
    int max_func_calls;
    int num_func_calls;
    int ntrial;
    double * x;
    double * trial_x;
    double * dx;
    int energy_initialized;
} SAstruct;

SAstruct localModel[MAX_THREADS];

struct drand48_data randThread[MAX_THREADS];

double drand_range(double min, double max) {
    double randCall;
    drand48_r(&(randThread[omp_get_thread_num()]),&randCall);
    double rand_value =  min+(max-min)*randCall;
    return rand_value;
}
void SA_copy(SAstruct * source, SAstruct * dest) {
    dest->energy = source->energy;
    dest->temp = source->temp;
    dest->step = source->step;
    dest->target = source->target;
    dest->allowance = source->allowance;
    dest->step_factor = source->step_factor;
    dest->cooling_factor = source->cooling_factor;
    dest->epsilon = source->epsilon;
    dest->n = source->n;
    dest->max_func_calls = source->max_func_calls;
    dest->num_func_calls = source->num_func_calls;
    dest->ntrial = source->ntrial;
    dest->energy_initialized = source->energy_initialized;
    for(int i=0;i<source->n;i++) {
        dest->x[i]=source->x[i];
        dest->trial_x[i]=source->trial_x[i];
        dest->dx[i]=source->dx[i];
    }
}

void SA_init(SAstruct * model, int n) {
    model->temp=1.0;
    model->step=0.1;
    model->target=0.5;
    model->allowance=0.1;
    model->step_factor=0.95;
    model->cooling_factor=0.95;
    model->epsilon=1.0e-7;
    model->max_func_calls = 10000000;
    model->num_func_calls = 0;
    model->n = n;
    model->ntrial = 100;
    model->x = (double *)calloc(sizeof(double),n);
    model->trial_x = (double *)calloc(sizeof(double),n);
    model->dx = (double *)calloc(sizeof(double),n);
    model->energy_initialized=0;
}

void SA_free(SAstruct * model) {
    free(model->x);
    free(model->trial_x);
    free(model->dx);
}


int SA_step(SAstruct * model, double func(int, double *, void *), void * opts) {
    int i;
    double trial_energy;

    if(!model->energy_initialized) {
        model->energy = func(model->n,model->x,opts);
        model->energy_initialized = 1;
    }

    // pick a random step;
    for(i=0;i<model->n;i++) {
        model->dx[i] = drand_range(-model->step,model->step);
    }

    // try out new value
    for(i=0;i<model->n;i++) {
        model->trial_x[i] = model->x[i] + model->dx[i];
    }
    trial_energy = func(model->n,model->trial_x,opts);
    model->num_func_calls++;
    

    double deltaE = trial_energy-model->energy;
    if(deltaE<0) {
        for(i=0;i<model->n;i++) {
            model->x[i] = model->trial_x[i];
            model->energy = trial_energy;
        }
        return 1;
    } else {
        if(drand_range(0.0,1.0)<exp(-deltaE/model->temp)) {
            for(i=0;i<model->n;i++) {
                model->x[i] = model->trial_x[i];
                model->energy = trial_energy;
            }
            return 1;
        } else {
            return 0;
        }
    }
    
}

int SA_adjust_step(SAstruct * model,
        double func(int, double *, void *), void * opts) {
    int iter,i; 
    int count_success = 0;
    double ratio;
    int num_func_calls = model->num_func_calls;
    int add_func_calls = 0;
    SAstruct * threadModel;
    double bestEnergy = model->energy;

    // NEED TO: split our model into n_threads different models
#pragma omp parallel private(threadModel) reduction(+:count_success) \
    reduction(+:add_func_calls)
{
    threadModel = &(localModel[omp_get_thread_num()]);
    SA_copy(model,threadModel);
// each walker does part of the random walk
#pragma omp for schedule(static)
    for(iter=0;iter<model->ntrial;iter++) {
        count_success += SA_step(threadModel,func,opts);
    }
    add_func_calls += threadModel->num_func_calls - num_func_calls;
// keep best walker
    if(threadModel->energy<bestEnergy) {
#pragma omp critical
      {
        bestEnergy = threadModel->energy;
        SA_copy(threadModel,model);
      }
    }
}
    model->num_func_calls = num_func_calls + add_func_calls;
    
    ratio = (double)count_success/model->ntrial;
    if(ratio<model->target-model->allowance) {
        // too greedy, make step smaller
        model->step *= model->step_factor;
        return -1;
    } else if(ratio>model->target+model->allowance) {
        // too timid, make step larger
        model->step /= model->step_factor;
        return 1;
    } else {
        // just right
        return 0;
    }
    
}


int SA_check_temperature(SAstruct * model,
        double func(int, double *, void *), void * opts) {

    int probe;

    probe = SA_adjust_step(model,func,opts);
    if(probe==0) {
        // thermal equilibrium reached, drop temperature
        model->temp*= model->cooling_factor;
        if(model->step<model->epsilon) {
            // if step size small at thermal equil, converge
            return 1;
        }
    }
    // check for convergence failure
    if(model->num_func_calls>model->max_func_calls) {
        printf("WARNING, FAILURE TO CONVERGE IN SA\n");
        return 2;
    }
    return 0;
    
}

int main(int argc, char ** argv) {
    int i;
    SAstruct model;
    SA_init(&model,20);

    int seed = time(NULL);
    printf("SEED %d\n", seed);
#pragma omp parallel
    srand48_r(seed+omp_get_thread_num(),&(randThread[omp_get_thread_num()]));

    // initialize guess
    for(i=0;i<model.n;i++) {
        model.x[i] = 1.0;
    }

    for(int i=0;i<MAX_THREADS;i++) { 
        SA_init(&(localModel[i]),model.n);
    }

    while(!SA_check_temperature(&model,funcN,NULL)) {
        // while not converged
            // while not at thermal equilibrium
                // adjust step
            // reduce temperature
    }

    // print output
    printf("SOLUTION %lf %lf\n",model.x[0],model.x[1]);
    printf("TOTAL CALLS %d\n",model.num_func_calls);

    SA_free(&model);
    for(int i=0;i<MAX_THREADS;i++) { 
        SA_free(&(localModel[i]));
    }
}
