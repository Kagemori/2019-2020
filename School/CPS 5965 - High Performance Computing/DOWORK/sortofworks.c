#include <fftw3.h>
#include <math.h>
int main(int argc, char ** argv)
{
    int N = 32;
    int i;
    fftw_complex *in, *out;
    fftw_plan p;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    for(i=0;i<N;i++) {
        in[i][0] = sin(5.1*i);
    }
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p); /* repeat as needed */
    for(i=0;i<N;i++) {
        printf("%lf \n",(out[i][0]*out[i][0]+out[i][1]*out[i][1]));
    }
    
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}
