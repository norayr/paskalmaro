ZCC     ?= zcc
ZFLAGS  ?= +zx
PTC     ?= ../../../ptc
PTCFLAGS ?= -m -i16

.PHONY: all clean

all: input.tap

input.tap: input.c io.c
	$(ZCC) $(ZFLAGS) -o input input.c io.c -create-app

input.c: input.p
	$(PTC) $(PTCFLAGS) < input.p > input.c

clean:
	rm -f input.c input.tap
