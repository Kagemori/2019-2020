////////////////////////////////////////////
// MPI Life 0.9
// Copyright 2002, David Joiner and
//   The Shodor Education Foundation, Inc.
////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include "Life.h"
#include "mpi.h"
#include <time.h>

#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds
#define NIL (0)       // A name for the void pointer

typedef unsigned int bool;
#define false 0;
#define true 1;

#define MPI_TORIGHT 0
#define MPI_TOLEFT 1

MPI_Status mpi_status;

int main(int argc, char ** argv) {
    int size;
    int rank;
    int ngrid;
    int ncols;
    int nrows;
    int max_count;
    int do_display;
    int count;
    int i,j;
    int ** grid;
    int ** next_grid;

    // Set up MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    random_initByTime(rank);

    // defaults
    ngrid=105;
    ncols=ngrid;
    nrows=ngrid;
    max_count=1000;
    do_display=1;
    
    // command line arguments
    if (argc > 1) {
        sscanf(argv[1],"%d",&nrows);
    }
    if (argc > 2) {
        sscanf(argv[2],"%d",&ncols);
    }
    if (argc > 3) {
        sscanf(argv[3],"%d",&max_count);
    }
    if (argc > 4) {
        sscanf(argv[4],"%d",&do_display);
    }
    if (do_display!=0) do_display=1;


    allocate_grid(ncols, nrows, &grid);
    allocate_grid(ncols, nrows, &next_grid);
    randomize_grid(ncols, nrows, grid, 0.25);
    if (do_display==1) 
        setupWindow(ncols, nrows);
    if (do_display==1)
        moveWindow(rank,size,ncols,nrows);


    bool done=false;
    count=0;
    while(!done) {
        if (count++>max_count) done=true;
        // output
        if (count%1==0&&do_display==1) doDraw(rank,ncols,nrows,grid);
        do_step(rank,size,ncols,nrows,grid,next_grid);
        //doDraw(rank,ncols,nrows,next_grid);
        //do_step(ncols,nrows,next_grid,grid);
        for (i=0;i<ncols+2;i++) {
            for (j=0;j<nrows+2;j++) {
                grid[i][j]=next_grid[i][j];
            }
        }
    }
    
    

    cleanup_grid(ncols, nrows, &grid);
    cleanup_grid(ncols, nrows, &next_grid);
    free_video();
    MPI_Finalize();


}

void do_step(int rank, int size, int ncols, int nrows, int ** grid,
        int ** next_grid) {
    int i,j,k,l,neighbors;
    
    // side by side grid
    // top and bottom we get from current cell.
    //left right and corners we get from neighboring grids.
    // start off with non blocking sends of each "row"
    // left is rank - 1 % size, right is rank + 1 % size.

    // copy sides
    int left_rank = (rank-1+size)%size;
    int right_rank = (rank+1)%size;
    
    if (left_rank>=rank) {
        MPI_Send(grid[1],nrows+2,MPI_INT,left_rank,MPI_TOLEFT,
            MPI_COMM_WORLD);
        MPI_Recv(grid[ncols+1],nrows+2,MPI_INT,right_rank,
            MPI_TOLEFT,
            MPI_COMM_WORLD, &mpi_status);
    } else {
        MPI_Recv(grid[ncols+1],nrows+2,MPI_INT,right_rank,
            MPI_TOLEFT,MPI_COMM_WORLD, &mpi_status);
        MPI_Send(grid[1],nrows+2,MPI_INT,left_rank,MPI_TOLEFT,
            MPI_COMM_WORLD);
    }
    
    if (right_rank>=rank) {
        MPI_Send(grid[ncols],nrows+2,MPI_INT,right_rank,MPI_TORIGHT,
            MPI_COMM_WORLD);
        MPI_Recv(grid[0],nrows+2,MPI_INT,left_rank,
            MPI_TORIGHT,
            MPI_COMM_WORLD, &mpi_status);
    } else {
        MPI_Recv(grid[0],nrows+2,MPI_INT,left_rank,
            MPI_TORIGHT,
            MPI_COMM_WORLD, &mpi_status);
        MPI_Send(grid[ncols],nrows+2,MPI_INT,right_rank,MPI_TORIGHT,
            MPI_COMM_WORLD);
    }
    
    // copy corners
	grid[0][0]=grid[0][nrows];
	grid[0][nrows+1]=grid[0][1];
	grid[ncols+1][0]=grid[ncols+1][nrows];
	grid[ncols+1][nrows+1]=grid[ncols+1][1];
	
	//copy top and bottom
	for (i=1;i<=ncols;i++) {
		grid[i][0]=grid[i][nrows];
		grid[i][nrows+1]=grid[i][1];
	}


	//update
	for (i=1;i<=ncols;i++) {
		for (j=1;j<=nrows;j++) {
			neighbors=0;
			for (k=i-1;k<=i+1; k++) {
				for (l=j-1;l<=j+1; l++) {
					if (!(k==i&&l==j)&&grid[k][l]>0) {
						neighbors++;
					}
				}
				if (neighbors>3) continue;
			}
			if (neighbors<2||neighbors>3) {
				next_grid[i][j]=0;
			} else if (grid[i][j]>0||neighbors==3) {
				next_grid[i][j]=grid[i][j]+1;
			}
		}
	}
}

typedef int * intarray;

void allocate_grid(int ncols, int nrows, int *** grid){
    int i,j;
    (*grid) = (int **) malloc(sizeof(intarray)*(ncols+2));
    for (i=0; i<ncols+2;i++) {
        (*grid)[i] = (int *) malloc(sizeof(int)*(nrows+2));
        for (j=0;j<nrows+2;j++) {
            (*grid)[i][j]=0;
        }
    }
}
void cleanup_grid(int ncols, int nrows, int *** grid){
    int i;
    for (i=0;i<ncols+2;i++) {
        free((*grid)[i]);
    }
    free(*grid);
}
void randomize_grid(int ncols, int nrows, int ** grid, double prob){
    int i,j;
    for (i=1;i<=ncols;i++) {
        for (j=1;j<=nrows;j++) {
            if (rand_double()<prob) {
                grid[i][j]=1;
            }
        }
    }
}

double rand_double() {
    return (double)rand()/(double)RAND_MAX;
}



// X information, at some point this should be cleaned up so
// that it does not use global variables

// setupWindow modified from the tutorial on
// http://tronche.com/gui/x/xlib-tutorial/
// by Christophe Tronche


Display *dpy;
int blackColor;
int whiteColor;
Window w;
GC gc;
Pixmap buffer;
Colormap theColormap;
int numXGrayscale=10;
XColor Xgrayscale[10];
int IMAGE_WIDTH=LIFE_IMAGE_WIDTH;
int IMAGE_HEIGHT=LIFE_IMAGE_HEIGHT;

void free_video() {
     XCloseDisplay(dpy);
}

void moveWindow(int rank,int size,int ncols, int nrows) {
      int posx, posy;
      posx = 50+(int)((double)rank/(double)size*(double)LIFE_IMAGE_WIDTH);
      posy = 50;
      XMoveWindow(dpy, w, posx, posy);
}


void setupWindow(int ncols, int nrows) {
      int i;
      // Open the display

      dpy = XOpenDisplay(NIL);
      assert(dpy);

      // Get some colors

      blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

      // Create the window
      if (nrows>ncols) {
         IMAGE_WIDTH = (int)((double)LIFE_IMAGE_WIDTH*(double)ncols/(double)nrows);
         IMAGE_HEIGHT = LIFE_IMAGE_HEIGHT;
      } else {
         IMAGE_HEIGHT = (int)((double)LIFE_IMAGE_HEIGHT*(double)nrows/(double)ncols);
         IMAGE_WIDTH = LIFE_IMAGE_WIDTH;
      }
      

      w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
				     IMAGE_WIDTH, IMAGE_HEIGHT, 0, blackColor,
				     blackColor);
      buffer = XCreatePixmap(dpy,DefaultRootWindow(dpy),
          IMAGE_WIDTH,IMAGE_HEIGHT,DefaultDepth(dpy,
          DefaultScreen(dpy)));
          
      theColormap = XCreateColormap(dpy, DefaultRootWindow(dpy),
          DefaultVisual(dpy,DefaultScreen(dpy)), AllocNone);
          
      for (i=0;i<numXGrayscale;i++) {
          int color = (int)((double)i*35535.0/(double)numXGrayscale)+30000;
          Xgrayscale[i].red=color;
          Xgrayscale[i].green=color;
          Xgrayscale[i].blue=color;
          XAllocColor(dpy,theColormap,&(Xgrayscale[i]));
      }

      // We want to get MapNotify events

      XSelectInput(dpy, w, StructureNotifyMask);

      // "Map" the window (that is, make it appear on the screen)

      XMapWindow(dpy, w);

      // Create a "Graphics Context"

      gc = XCreateGC(dpy, w, 0, NIL);

      // Tell the GC we draw using the white color

      XSetForeground(dpy, gc, whiteColor);

      // Wait for the MapNotify event

      for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    if (e.type == MapNotify)
		  break;
      }

}

void doDraw(int rank, int ncols, int nrows, int ** grid) {

    int x1,x2,y1,y2; 
    int i,j;
    char string[2];
    sprintf(string,"%d",rank);
    
    XSetForeground(dpy, gc, blackColor);
    XFillRectangle(dpy,buffer,gc,0,0,IMAGE_WIDTH,IMAGE_HEIGHT);
    int rect_width=(int)((double)IMAGE_WIDTH/(double)(ncols+1));
    int rect_height=(int)((double)IMAGE_HEIGHT/(double)(nrows+1));
    for (i=1;i<=ncols;i++) {
        x1 = (int)((double)(i-1)/(double)(ncols+1)*(double)IMAGE_WIDTH);
        for (j=1;j<=nrows;j++) {
            y1 = (int)((double)(j-1)/(double)(nrows+1)*
                (double)IMAGE_HEIGHT);
            if (grid[i][j]>0) {
                int life =grid[i][j];
                if (life>numXGrayscale-1) life=numXGrayscale-1;
                XSetForeground(dpy, gc, Xgrayscale[life].pixel);
            } else {
                XSetForeground(dpy, gc, blackColor);
            }
            XFillRectangle(dpy,buffer,gc,x1,y1,rect_width,rect_height);
         }
     }
     XSetForeground(dpy,gc,blackColor);
     XFillRectangle(dpy,buffer,gc,10,10,15,15);
     XSetForeground(dpy,gc,whiteColor);
     XDrawRectangle(dpy,buffer,gc,10,10,15,15);
     XDrawString(dpy,buffer,gc,12,23,string,2);
     
     XCopyArea(dpy, buffer, w, gc, 0, 0,
         IMAGE_WIDTH, IMAGE_HEIGHT,  0, 0);
     XFlush(dpy);
	  
}

void random_initByTime(int rank) {
    time_t ltime;

    time(&ltime);
    srand((unsigned) ltime + 100*rank);
}
