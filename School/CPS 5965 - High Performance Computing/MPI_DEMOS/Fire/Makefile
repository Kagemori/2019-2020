
CC        = mpicc

LIBS      = -lX11 -lm

CFLAGS    = -I/usr/X11R6/include
LDFLAGS   = -L/usr/X11R6/lib64 $(LIBS)

PROGRAM   = Fire			# name of the binary
SRCS      = fire_reduce.c X-graph.c
OBJS      = $(SRCS:.c=.o)		# object file

.SUFFIXES: .c .o

.cpp.o: 
	$(CC) -c $(CFLAGS) $<

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
