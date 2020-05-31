//////////////////////////////////////////////////////////
// GalaxSee (version MPI 0.9)
// Copyright 1997 - 2002
// David A. Joiner and the Shodor Education Foundation
//////////////////////////////////////////////////////////

#include <math.h>
#include "modeldata.h"
#include "mpidata.h"
#include "mpi.h"
#include <stdio.h>

extern mpidata g_mpi;


void derivs_client() {

    int npoints;
    int npercpu;
    double gnorm;
    double * mass;
    double * shield_rad;
    double * x;

    int pass_numbers[2];
    int buffer_pos;
    int buffer_size;


    // Get information from server
    MPI_Recv(pass_numbers,2,MPI_INT,0,MPIDATA_PASSNUMBER,
        MPI_COMM_WORLD,&g_mpi.status);
    npoints=pass_numbers[0];
    npercpu=pass_numbers[1];

    buffer_size=(8*npoints+1)*sizeof(double);
    buffer_pos=0;
    if (g_mpi.buffer_size!=buffer_size) {
        if (g_mpi.buffer!=NULL) {
            delete g_mpi.buffer;
        }
        g_mpi.buffer_size=buffer_size;
        g_mpi.buffer = new char[buffer_size];
    }

    mass = new double[npoints];
    shield_rad = new double[npoints];
    x = new double[npoints*6];
    MPI_Recv(g_mpi.buffer,buffer_size,MPI_PACKED,0,MPIDATA_PASSX,
       MPI_COMM_WORLD,&g_mpi.status);
    MPI_Unpack(g_mpi.buffer,buffer_size,&buffer_pos,
        &gnorm,1,MPI_DOUBLE,MPI_COMM_WORLD);
    MPI_Unpack(g_mpi.buffer,buffer_size,&buffer_pos,
        mass,npoints,MPI_DOUBLE,MPI_COMM_WORLD);
    MPI_Unpack(g_mpi.buffer,buffer_size,&buffer_pos,
        shield_rad,npoints,MPI_DOUBLE,MPI_COMM_WORLD);
    MPI_Unpack(g_mpi.buffer,buffer_size,&buffer_pos,
        x,6*npoints,MPI_DOUBLE,MPI_COMM_WORLD);

    // Do stuff and return values
    double * retval = new double[npercpu*6];
    for (int i=0;i<npercpu*6;i++) {retval[i]=0.0;}
    double rad,rad2,dcon,diffx,diffy,diffz;
    int block = npoints/g_mpi.size;
    for(int i=0;i<npercpu;i++){
        int ireal = g_mpi.rank*(block)+i;
        for(int j=0;j<npoints;j++){
          if (ireal!=j) {
            rad2=pow((x[ireal]-x[j]),2)+
                pow((x[npoints+ireal]-x[npoints+j]),2)+
                pow((x[2*npoints+ireal]-x[2*npoints+j]),2);
            rad=sqrt(rad2);
            dcon=gnorm/(rad*rad2);
            diffx=(x[j]-x[ireal])*dcon;
            diffy=(x[npoints+j]-x[npoints+ireal])*dcon;
            diffz=(x[2*npoints+j]-x[2*npoints+ireal])*dcon;

            if (rad>shield_rad[j]) {
                retval[3*npercpu+i]+=diffx*mass[j];
                retval[4*npercpu+i]+=diffy*mass[j];
                retval[5*npercpu+i]+=diffz*mass[j];
            }
          }  
        }
        retval[i]=x[3*npoints+ireal];
        retval[npercpu+i]=x[4*npoints+ireal];
        retval[2*npercpu+i]=x[5*npoints+ireal];
    }
    
    
    MPI_Send(retval,npercpu*6,MPI_DOUBLE,0,
        MPIDATA_DONEDERIVS,MPI_COMM_WORLD);
    
    delete mass;
    delete shield_rad;
    delete x;
    delete retval;
}
