
CC        = mpicxx

LIBS      = -lX11 -lm

CFLAGS    = -I/usr/X11R6/include
LDFLAGS   = -L/usr/X11R6/lib64  $(LIBS)

PROGRAM   = GalaxSee			# name of the binary
SRCS      = Gal.cpp derivs.cpp diffeq.cpp modeldata.cpp point.cpp derivs_client.cpp
OBJS      = $(SRCS:.cpp=.o)		# object file

.SUFFIXES: .c .o

.cpp.o: 
	$(CC) -c $(CFLAGS) $<

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
