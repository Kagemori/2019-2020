#include <math.h>
#include <stdio.h>
#include <time.h>

double f(double x){
	return sin(x);
}

int main(int argc, char ** argv){
	double sum = 0.0;
	long i;
	long n = 100;
	double a = 0.0;
	double b = M_PI;
	double x = a;
	double dx;
	clock_t start, end;
	double time;
	
	if(argc>1){
		sscanf(argv[1],"%ld",&n);
	}
	printf("Running with N = %ld\n",n);

	dx = (b-a)/n;
	
	start = clock();

#pragma omp parallel for reduction(+:sum) private(x)
	for(i = 1;i<n;i++){
		x = a + i*dx;
		sum += f(x);
	}
	sum += 0.5*(f(a)+f(b));
	sum *= dx;

	end = clock();

	time = (end-start)/(double)CLOCKS_PER_SEC;
	printf("Elapsed Time = %lf\n",time);
	
	printf("INTEGRAL = %lf\n",sum);
}
