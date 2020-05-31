//////////////////////////////////////////////////////////
// GalaxSee (version MPI 0.9)
// Copyright 1997 - 2002
// David A. Joiner and the Shodor Education Foundation
//////////////////////////////////////////////////////////

#include "mpi.h"

#define MPIDATA_EXIT_TAG 1000
#define MPIDATA_DODERIVS 10
#define MPIDATA_PASSNUMBER 100
#define MPIDATA_PASSNUMBERPER 101
#define MPIDATA_PASSGNORM 110
#define MPIDATA_PASSMASS 120
#define MPIDATA_PASSSHIELD 125
#define MPIDATA_PASSX 130

#define MPIDATA_DONEDERIVS 200

struct mpidata {
    MPI_Status status;
    int rank;
    int size;
    char * buffer;
    int buffer_size;
};
