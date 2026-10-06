CL65    ?= cl65
CFLAGS  ?= -t atmos -O
PTC     ?= ../../../ptc
PTCFLAGS ?= -m -i16

.PHONY: all clean

all: clrscr.tap

clrscr.tap: clrscr.c rom.s
	$(CL65) $(CFLAGS) -o clrscr.tap clrscr.c rom.s

clrscr.c: clrscr.p
	$(PTC) $(PTCFLAGS) < clrscr.p > clrscr.c

clean:
	rm -f clrscr.c clrscr.tap
