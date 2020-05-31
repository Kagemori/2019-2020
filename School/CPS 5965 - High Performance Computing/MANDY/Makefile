CC            = gcc
CFLAGS	      = -Wall -fopenmp ## ADD OPENMP FLAG
LDFLAGS	      = -o  
LIBS          =  -lm -fopenmp ## ADD OPENMP FLAG

###### DEBUG OPTIONS
#CFLAGS += -g
#CFLAGS += -pg -g

###### OPTIMIZATION OPTIONS
CFLAGS += -O3

###### LIBGD OPTIONS
CFLAGS += -DUSE_GD -I/share/apps/libgd/include
LIBS +=  -L/share/apps/libgd/lib -ljpeg -lpng -lz -lgd

OBJS	      = gdimage.o
OBJS += mandy.o
#OBJS += mandy_omp_starter.o

PROGRAM		= mandy

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
