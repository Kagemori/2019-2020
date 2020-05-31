////////////////////////////////////////////
// MPI Life 0.9
// Copyright 2002, David Joiner and
//   The Shodor Education Foundation, Inc.
////////////////////////////////////////////


#define LIFE_IMAGE_WIDTH 500
#define LIFE_IMAGE_HEIGHT 500

void allocate_grid(int ncols, int nrows, int *** grid);
void cleanup_grid(int ncols, int nrows, int *** grid);
void free_video();
void randomize_grid(int ncols, int nrows, int ** grid, double prob);
double rand_double();

void setupWindow(int , int);
void moveWindow(int rank,int size,int ncols, int nrows) ;
void doDraw(int rank, int ncols, int nrows, int ** grid);
void do_step(int rank,int size,int ncols,int nrows,int ** grid,int ** next_grid);

void random_initByTime(int rank) ;


