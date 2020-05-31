#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
 
int main(int argc, char* argv[])
{
    int iter = 1000000;
    int rank, i;
    double x, y, z, pi;
    int count = 0;
    int recount, reiter;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   //get rank of node's process
 
    for (i=0; i<iter; ++i)
    {
        srand(time(NULL)+rank);           //seed using current time and rank of process
        x = (double)random()/RAND_MAX;
        y = (double)random()/RAND_MAX;
        z = sqrt((x*x)+(y*y));              //Check if the number is inside the circle
        if (z<=1)
        {
            ++count; //if its in the circle, count it
        }
    }
     
 
    MPI_Reduce(&count, &recount,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Reduce(&iter,&reiter,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
     
    if (myid == 0) //the root process calculates the final guess for pi using the reduction prior
    {
        pi = 4*((double)recount/(double)reiter);
        printf("Pi: %f\n%i\n%d\n", pi, recount, reiter); 
    }
 
    MPI_Finalize();
    return 0;
}