#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char ** argv) {

// calculate energy of N particles 
//   subject to E = \sum_i \sum_j>i 1/|x_j-x_i|

    int N = 100;
    double L = 10.0;
    double * x;
    int i,j;
    double E;
    int iarg;

    printf("USAGE ./energy [N [L]]\n");
    iarg=1;
    if(argc>iarg) sscanf(argv[iarg++],"%d",&N);
    if(argc>iarg) sscanf(argv[iarg++],"%lf",&L);
    printf("USING N = %d   L = %lf\n",N,L);

    x = (double *)malloc(sizeof(double)*N);
    srand(time(NULL));

    // random initial state
    for(i=0;i<N;i++) {
        x[i] = L*(double)rand()/(double)RAND_MAX;
    }


    E=0;
    // calculate energy
    for(i=0;i<N;i++) {
        for(j=i+1;j<N;j++) {
            E += 1.0/fabs(x[j]-x[i]);
        }
    }

    printf("E= %lf\n",E);
    



    free(x);

}
