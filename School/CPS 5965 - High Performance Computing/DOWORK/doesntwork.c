#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>

int main(int argc, char ** argv) {
    int nx = 800;
    int i;
    double *x=(double *)malloc(sizeof(double)*nx);
    double *k=(double *)malloc(sizeof(double)*nx);
    double *y=(double *)malloc(sizeof(double)*nx);
    double xmax,xmin;
    double dx;
    fftw_complex * out =(fftw_complex *)
        fftw_malloc((nx/2+1)*sizeof(fftw_complex));
    fftw_plan p;
    fftw_plan pi;

    p = fftw_plan_dft_r2c_1d(nx, y, out, FFTW_MEASURE);
    pi = fftw_plan_dft_c2r_1d(nx, out, y, FFTW_MEASURE);

    xmax=10.0;
    xmin=-10.0;
    dx = (xmax-xmin)/(double)(nx-1);
    x[0] = xmin;
    for(i=1;i<nx-1;i++) {
        x[i] = xmin+(double)i*dx;
    }
    x[nx-1] = xmax;

    // fft frequencies
    k[0]=0;
    for(i=1;i<nx/2;i++) {
        k[i] = (double)i/(xmax-xmin);
        k[nx-i]=-k[i];
    }
    k[nx/2]=0.5/(xmax-xmin);
    
    for (i=0;i<nx;i++) 
        y[i] = sin(5*x[i]);

    fftw_execute(p);
    fftw_execute(pi);

    for(i=0;i<nx/2;i++) {
        printf("%lf   %lf\n",k[i],(out[i][0]*out[i][0]+out[i][1]*out[i][1]));
    }

    fftw_destroy_plan(p);
    fftw_destroy_plan(pi);
    fftw_free(out);
    free(x);
    free(k);
    free(y);

    return 0;
}
