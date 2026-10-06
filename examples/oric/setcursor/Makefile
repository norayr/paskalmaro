CL65    ?= cl65
CFLAGS  ?= -t atmos -O
PTC     ?= ../../../ptc
PTCFLAGS ?= -m -i16

.PHONY: all clean

all: setcursor.tap

setcursor.tap: setcursor.c rom.s
	$(CL65) $(CFLAGS) -o setcursor.tap setcursor.c rom.s

setcursor.c: setcursor.p
	$(PTC) $(PTCFLAGS) < setcursor.p > setcursor.c

clean:
	rm -f setcursor.c setcursor.tap
