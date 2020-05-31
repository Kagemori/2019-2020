//////////////////////////////////////////////////////////
// GalaxSee (version MPI 0.9)
// Copyright 1997 - 2002
// David A. Joiner and the Shodor Education Foundation
//////////////////////////////////////////////////////////


#ifndef _DIFFEQ
#define _DIFFEQ

#include <math.h>
#include <stdlib.h>

#define IMETHOD_EULER 0
#define IMETHOD_IEULER 1
#define IMETHOD_RKUTTA4 2

class diffeq{
  public:
   int neq;
   double *x;
   double time;

  public:
   double * der;
   double * x_old;
   double * der_old;
   double * k1;
   double * k2;
   double * k3;
   double * k4;

  public:
   void init(int j);
   void cleanup();
   void updateEuler(double, void no_name(int,double,double*,double*));
   void updateIEuler(double, void no_name(int,double,double*,double*));
   void updateRKutta4(double, void no_name(int,double,double*,double*));
};

class cart3d{
  public:
   int n;
   double * x;
   double * y;
   double * z;

   void init(int j);

};

class dynamic : public diffeq {
  public:
   int npoints;
   cart3d pos;
   cart3d vel;
   cart3d xder;
   cart3d vder;

   void init(int);
};

void mapPoints(int, int, double *, double *, cart3d *,
      cart3d *, cart3d *, cart3d*);


#endif
