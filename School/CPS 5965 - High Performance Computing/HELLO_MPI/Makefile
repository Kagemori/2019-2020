CC            = /opt/mpich/intel/bin/mpicc
FC            = /opt/mpich/intel/bin/f77
#FC	      = g77
#CC            = gcc
FFLAGS	      = -Wall -O3 -msse3
#CFLAGS	      = -Wall  -DCSD_MPI
CFLAGS	      = -Wall -O3 -msse3
LDFLAGS	      = -msse3  -o 
LIBS          = -lm

OBJS	      = hello.o

PROGRAM		= hello

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)

rmout:;		rm -f hello *.o
