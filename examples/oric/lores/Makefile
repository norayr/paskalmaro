PTC      ?= ../../../ptc
PTCFLAGS ?= -m -i16
CL65     ?= cl65
LCC65    ?= lcc65
HOSTCPP  ?= cpp
OSDK     ?=
OSDKADDR ?= 0x600
OSDKMACROS ?= $(firstword $(wildcard $(OSDK)/macro/macros.h $(OSDK)/macro/MACROS.H))

.PHONY: all osdk frontend clean
all: demo.tap

demo.c: demo.p $(PTC)
	$(PTC) $(PTCFLAGS) < $< > $@
lores.c: lores.p $(PTC)
	$(PTC) $(PTCFLAGS) < $< > $@

demo.tap: demo.c lores.c oric.c
	$(CL65) -t atmos -O -o $@ demo.c lores.c oric.c

# This stage needs only the installed compiler, not an OSDK SDK data tree.
frontend: demo.c2 lores.c2 oric.c2
%.c2: %.c
	$(HOSTCPP) -P -nostdinc -DPTC_OSDK -D__16BIT__ -D__NOFLOAT__ -DATMOS $< > $*.pre.c
	$(LCC65) -N$* $*.pre.c > $@

# A complete SDK supplies macro/macros.h and lib/library.ndx + runtime.
osdk: demo-osdk.tap
%.s: %.c2
	test -f "$(OSDKMACROS)"
	macrosplitter -M"$(OSDKMACROS)" $< $@

linked.s: demo.s lores.s oric.s
	test -f "$(OSDK)/lib/library.ndx"
	link65 -d "$(OSDK)/lib/" -i "$(OSDK)/lib/" -o $@ -f -q demo.s lores.s oric.s

demo-osdk.bin: linked.s
	xa -I"$(OSDK)/lib" linked.s -o $@ -bt $(OSDKADDR) -DASSEMBLER=XA -W -C

demo-osdk.tap: demo-osdk.bin
	header -nLORES -a1 $< $@ $(OSDKADDR)

clean:
	rm -f demo.c lores.c *.pre.c *.c2 demo.s lores.s oric.s linked.s *.o *.bin *.tap
