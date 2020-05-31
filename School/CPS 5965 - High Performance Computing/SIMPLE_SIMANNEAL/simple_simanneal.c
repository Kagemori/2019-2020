#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


double func(int n, double * x_arr, void * v_opts) {
     // sample function
     double x = x_arr[0];
     double y = x_arr[1];
     //double f = -sin(x)*sin(y)*exp(-(x*x+y*y))+1.0e-20*(x*x+y*y);
     double f = x*x+y*y;
     return f;
}


void print_array(int n, double * x) {
    int i;
    for(i=0;i<n;i++) {
        printf("%lf ",x[i]);
    }
    printf("\n");
}

double drand_range(double min, double max) {
    double rand_value =  min+(double)rand()/RAND_MAX*(max-min);
    return rand_value;
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

// He inserted SA_copy here

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

// NEED TO: split our model into n_threads different models
    for(iter=0;iter<model->ntrial;iter++) {
        count_success += SA_step(model,func,opts);
    }
    
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
    SAstruct model;
    SA_init(&model,2);

    int seed = time(NULL);
    printf("SEED %d\n", seed);
    srand(seed);

    // initialize guess
    model.x[0] = 1.0;
    model.x[1] = 2.0;

    while(!SA_check_temperature(&model,func,NULL)) {
        // while not converged
            // while not at thermal equilibrium
                // adjust step
            // reduce temperature
			
		
    }

    // print output
    printf("SOLUTION %lf %lf\n",model.x[0],model.x[1]);
    printf("TOTAL CALLS %d\n",model.num_func_calls);

    SA_free(&model);
}
