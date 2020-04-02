CFLAGS=-O0 -ggdb -Wall
LDLIBS=-lX11
GENERATED=x11launch x11launch.1 *.o
JUNK=*~ *.bak

%.1:	%
	help2man -N --name="`grep -EA1 '##*[[:space:]]$<$$' README.md | tail -1`" ./$< -o $@

all:	x11launch x11launch.1

clean:
	rm -f $(JUNK) $(GENERATED)
