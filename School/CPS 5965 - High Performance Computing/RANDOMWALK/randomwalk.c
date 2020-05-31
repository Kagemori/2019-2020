#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char ** argv) {

    int nTrials = 100;
    double stepSize = 1.0;
    double radius = 10.0;
    double sum;
    double mean;
    double meanBuffer;
    int i;
    int iarg;
    int rank = 0;
    int size = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    printf("USAGE ./randomwalk [NTRIALS [RADIUS [STEPSIZE]]]\n");
    iarg=1;
    if(argc>iarg) sscanf(argv[iarg++],"%d",&nTrials);
    if(argc>iarg) sscanf(argv[iarg++],"%lf",&radius);
    if(argc>iarg) sscanf(argv[iarg++],"%lf",&stepSize);
    printf("USING NTRIALS = %d   RADIUS = %lf STEPSIZE = %lf\n",
        nTrials, radius,stepSize);

    srand(time(NULL));

    sum = 0.0;
    for(i=0;i<nTrials;i++) {
        int nSteps=0;
        double x = 0.0;
        double y = 0.0;
        while(x*x+y*y<radius*radius) {
            double theta = M_PI*2.0*(double)rand()/(double)RAND_MAX;
            x += stepSize*cos(theta);
            y += stepSize*sin(theta);
            nSteps++;
        }
        sum += nSteps;
    }
    mean = sum/nTrials;
    MPI_Reduce(&mean,&meanBuffer,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank == 0){
	mean = meanBuffer;
    	printf("Mean number of steps = %lf\n",mean);
    }

    MPI_Finalize();
}
