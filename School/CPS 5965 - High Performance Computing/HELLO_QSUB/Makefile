CC            = gcc
CFLAGS	      = -Wall -O3 -msse3
LDFLAGS	      = -msse3 -o 
LIBS          = -lm 

OBJS	      = hello.o

PROGRAM		= hello

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)

rmout:;		rm -f hello *.o
