#include <fftw3.h>
#include <stdlib.h>
#include <math.h>
int main(int argc, char ** argv)
{
    int N = 32;
    int i;
    double * x = (double *) malloc(sizeof(double)*N);
    double * k = (double *) malloc(sizeof(double)*N);
    fftw_complex *in, *out;
    fftw_plan p;


    for(i=0;i<N;i++) 
    {
        x[i] = i*10.0/(N-1);
    }
    k[0]=0;
    for(i=1;i<N/2;i++) {
        k[i] = (double)i/10;
        k[N-i] = -(double)i/10;
    }
    k[N/2] = (double)N/(2*10);

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    for(i=0;i<N;i++) {
        in[i][0] = sin(2*M_PI*x[i]);
    }
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p); /* repeat as needed */
    for(i=0;i<N;i++) {
        printf("%lf %lf \n",k[i],(out[i][0]*out[i][0]+out[i][1]*out[i][1]));
    }
    
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}
