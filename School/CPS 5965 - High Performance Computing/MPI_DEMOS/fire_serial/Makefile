CC            = gcc
CFLAGS	      = -Wall -O3
LDFLAGS	      = -o 
LIBS          = -lm

OBJS	      = fire_serial.o

PROGRAM		= fire

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)

rmout:;		rm -f fire *.o
