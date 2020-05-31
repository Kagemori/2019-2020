#include <stdio.h>
#include <math.h>
#include <time.h>

double * linspace(double min, double max, int n) {
	double * x = (double *)malloc(sizeof(double)*n);
	double dx = (max-min)/(double)(n-1);
	int i;
	x[0] = min;
	for(i=1;i<n-1;i++) {
		x[i] = min+(double)i*dx;
	}
	x[n-1] = max;
	return x;
}

double * zeros(int n) {
	int i;
	double * x = (double *)malloc(sizeof(double)*n);
	for(i=0;i<n;i++) x[i]=0;
	return x;
}

int main(int argc, char ** argv) {
	int n = 800;
	int nt = 64000;
	double * x = linspace(0,10,n);
	double * V = zeros(n);
	double * A = zeros(n);
	double * dVdt = zeros(n);	
	int i,it;
	double dx;
	double dxsquared;
	double * tgrid=linspace(0,10,nt);;
	double dt; 
	clock_t t;
	
	for(i=0;i<n;i++) {
		A[i] = exp(-(x[i]-5.0)*(x[i]-5.0));
	}
	dx = x[1]-x[0];
	dxsquared = dx*dx;
	dt = tgrid[1]-tgrid[0];

	//checking time for the nested for loop
	t = clock(); //start time
	for(it = 0;it<nt;it++) {
		for(i=1;i<n-1;i++) {
			dVdt[i] = (A[i+1]+A[i-1]-2.0*A[i])/dxsquared;
		}
		for(i=0;i<n;i++) {
			V[i] = V[i] + dVdt[i]*dt;
			A[i] = A[i] + V[i]*dt;
		}
	}
	t = clock() - t; //current time minus time prior to the for loop
	double time = ((double)t)/CLOCKS_PER_SEC;
	
	printf("NESTED FOR LOOP TIME IN SECONDS: %f\n",time);
	printf("MIDPOINT A = %lf\n",A[n/2]);
	free(x);
	free(V);
	free(A);
	free(dVdt);

}
