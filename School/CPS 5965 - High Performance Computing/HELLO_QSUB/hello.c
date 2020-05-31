#include <stdio.h>
#include <math.h>

double * linspace(double min, double max, int n){
  max = 10.0;
  min =- 10.0;
  dx = (max-min)/(double)(nx-1);
  x[0] = min;
  for(i=1;i<nx-1;i++){
    x[i] = min+(double)i*dx;
  }
  x[nx-1] = max;
  return x;
}

double * zeroes(int n){
  for(int i = 0; i < n; i++;){
    arr[i] = 0; 
  }
}

int main(int argc, char **argv) {
  n = 800;
  x = linspace(0,10,n)
  V = zeroes(n);
  A = exp(-(x-5.0)**2);
  dVdt = 0.0*x;

  dx = x[1]-x[0];
  dxsquared = dx*dx;
  nt = 64000;
  tgrid = linspace(0,10,nt);
  dt = tgrid[1]-tgrid[0];

  for t in tgrid:
    for i in range(1,n-1):
        dVdt[i] = (A[i+1]+A[i-1]-2.0*A[i])/dxsquared
    V = V + dVdt*dt
    A = A + V*dt

   
plt.plot(x,A)
plt.show()
}
