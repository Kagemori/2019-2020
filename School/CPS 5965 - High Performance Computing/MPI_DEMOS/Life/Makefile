
CC        = mpicc

#
# Modify TOPDIR if you use your own include files and library files
#

LIBS      = -lX11 -lm


CFLAGS    = 
LDFLAGS   = -L/usr/X11R6/lib64 $(LIBS)

PROGRAM   = Life
SRCS      = Life.c
OBJS      = $(SRCS:.c=.o)		# object file

#
# Rules
#

.SUFFIXES: .c .o

.cpp.o: 
	$(CC) -c $(CFLAGS) $<

#
# Targets
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)

