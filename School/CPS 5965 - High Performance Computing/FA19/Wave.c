#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main(int argc, char ** argv){
	int n = 2;
	double * x = linspace(-10.0,10.0,n);
	double * V = zeros(n);
	double * A = zeros(n);
	
	int l;
	for(l = 0; l < sizeof(A); l++){
		A[l] = exp(pow(((x[l]-5.0)*-1),2));
	}
	
	double * dVdt = zeros(n);
	double dx = x[1] - x[0];
	double dxsquared = dx*dx;
	int nt = 20;
	double * tgrid = linspace(-10.0,10.0,nt);
	double dt = tgrid[1]-tgrid[0];
	
	int t,i,j,k;
	for(t = 0; t < sizeof(tgrid); t++){
		for(i = 2; i <= n-1; i++){
			dVdt[i] = (A[i+1]+A[i-1]-2.0*A[i])/dxsquared;
		}
		for(j = 0; j < sizeof(A);j++){
			V[j] = V[j] * dt;
			A[j] = A[j] + V[j];
		}
		for(k = 0; k < sizeof(V);k++){
			dVdt[k] = dVdt[k] * dt;
			V[k] = V[k] + dVdt[k];
		}
	}
	int loop;
	for(loop = 0; loop < sizeof(A); loop++){
		printf("%d %d \n",x[loop],A[loop]);
	}
}
