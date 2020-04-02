CFLAGS=-O0 -ggdb -Wall
LDLIBS=-lX11

all:	x11launch

clean:
	rm -f *.o *~ *.bak x11launch
