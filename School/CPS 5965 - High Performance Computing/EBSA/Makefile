CC            = gcc 
CFLAGS	      = -Wall -O3 -fopenmp
LDFLAGS	      = -O3 -fopenmp -o 
LIBS          = -lm 

OBJS	      = ebsa.o\

PROGRAM		= ebsa

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)

clean:;		rm -f $(OBJS) $(PROGRAM)
