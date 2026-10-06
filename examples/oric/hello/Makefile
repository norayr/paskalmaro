CL65    ?= cl65
CFLAGS  ?= -t atmos -O
PTC     ?= ../../../ptc
PTCFLAGS ?= -m -i16

.PHONY: all clean

all: hello.tap

hello.tap: hello.c rom.s
	$(CL65) $(CFLAGS) -o hello.tap hello.c rom.s

hello.c: hello.p
	$(PTC) $(PTCFLAGS) < hello.p > hello.c

clean:
	rm -f hello.c hello.tap
