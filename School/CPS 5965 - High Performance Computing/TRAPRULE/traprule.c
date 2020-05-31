#include <math.h>
#include <stdio.h>
#include <time.h>

double f(double x) {
     return sin(x);
}

int main(int argc, char ** argv) {
    double sum=0.0;
    double sum_global=0.0;
    long i;
    long n = 100;
    double a = 0.0;
    double b = M_PI;
    double x = a;
    double dx;
    clock_t start,end;
    double elapsed_time;


    if(argc>1) {
        sscanf(argv[1],"%ld",&n);
    }
    printf("Running with N=%ld\n",n);


    dx = (b-a)/n;

    start = clock();


#pragma omp parallel private(x,sum,i)
{
#pragma omp for
    for(i = 1;i<n;i++) {
        x = a + i*dx;
        sum += f(x);
    }
#pragma omp critical
    sum_global += sum;
}
    sum = sum_global+ 0.5*(f(a)+f(b));
    sum *= dx;

    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);


    printf("INTEGRAL = %lf\n",sum);

}
