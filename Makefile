CFLAGS=-O0 -ggdb -Wall -Wno-unused
LDLIBS=-lX11
GENERATED=x11launch GeomTest x11launch.1 *.o
JUNK=*~ *.bak

%.1:	%
	help2man -N --name="`grep -EA1 '##*[[:space:]]$<$$' README.md | tail -1`" ./$< -o $@

all:	x11launch x11launch.1 test

test:
	./x11launch 'click|cmdclick' -v 'immediate||cmdimmed' -b 5 'SEP' -p blue -i khaki '||immed no label' -g 1x0-0+0 'next is pipline separator|just command' '|' 'that WAS pipleine||another imediate' -d

clean:
	rm -f $(JUNK) $(GENERATED)
