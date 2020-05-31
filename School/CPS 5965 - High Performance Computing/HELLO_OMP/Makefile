CC            = icc
CFLAGS	      = -Wall -O3 -msse3 -openmp
LDFLAGS	      = -msse3 -openmp  -o 
LIBS          = -lm 

OBJS	      = hello_omp.o

PROGRAM		= hello_omp

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)

rmout:;		rm -f hello *.o
