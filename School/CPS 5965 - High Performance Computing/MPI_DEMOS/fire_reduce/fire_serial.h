#ifndef FIRE
#define FIRE

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define UNBURNT 0
#define SMOLDERING 1
#define BURNING 2
#define BURNT 3

#define true 1
#define false 0

typedef int boolean;

void seed_by_time(int);
int ** allocate_forest(int);
void initialize_forest(int, int **);
double get_percent_burned(int, int **);
void delete_forest(int, int **);
void light_tree(int **,int,int);
boolean forest_is_burning(int, int **);
void forest_burns(int, int **,double);
void burn_until_out(int,int **,double,int,int);
void print_forest(int, int **);
boolean fire_spreads(double);

#define min(X,Y) ((X) < (Y) ? (X) : (Y))

#endif
