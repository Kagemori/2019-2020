#include <stdio.h>
#include <mpi.h>

#define SEND_SIZE 2

int main(int argc,char ** argv) {

    int size,rank;
    int i;
    int a[SEND_SIZE];
    int b[SEND_SIZE*3];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if (size!=3) {
        printf("\nThe allgather_test example should be run with 3 processes\n");
        exit(0);
    }

    for (i=0;i<SEND_SIZE;i++) a[i]=i+rank*SEND_SIZE;
    for (i=0;i<SEND_SIZE;i++)
        printf("rank %d - a[%d]=%d\n",rank,i,a[i]);

    MPI_Allgather (a,SEND_SIZE,MPI_INT,
                   b,SEND_SIZE,MPI_INT,
                   MPI_COMM_WORLD);

    for (i=0;i<size*SEND_SIZE;i++)
        printf("rank %d - b[%d]=%d\n",rank,i,b[i]);

    MPI_Finalize();

}
