PTC      ?= ../../ptc
CC       ?= cc
CFLAGS   ?= -std=c89 -O2
CL65     ?= cl65
LCC65    ?= lcc65
HOSTCPP  ?= cpp
RUNTIME  := ../../runtime
OSDK     ?=
OSDKADDR ?= 0x600
OSDKMACROS ?= $(firstword $(wildcard $(OSDK)/macro/macros.h $(OSDK)/macro/MACROS.H))

.PHONY: all cc65 osdk frontend clean
all: console-demo
cc65: console.tap
osdk: console-osdk.tap
frontend: console.c2 console_input.c2 console_output.c2 console_adapter.c2

console-host.c: demo.p $(PTC)
	$(PTC) -c < $< > $@
console.c: demo.p $(PTC)
	$(PTC) -c -i16 < $< > $@

console-demo: console-host.c $(RUNTIME)/ptc_console.h $(RUNTIME)/ptc_console_input.c $(RUNTIME)/ptc_console_output.c $(RUNTIME)/ptc_console_host.c
	$(CC) $(CFLAGS) -I$(RUNTIME) -o $@ console-host.c $(RUNTIME)/ptc_console_input.c $(RUNTIME)/ptc_console_output.c $(RUNTIME)/ptc_console_host.c

console.tap: console.c $(RUNTIME)/ptc_console.h $(RUNTIME)/ptc_console_input.c $(RUNTIME)/ptc_console_output.c $(RUNTIME)/ptc_console_oric.c
	$(CL65) -t atmos -O -I$(RUNTIME) -o $@ console.c $(RUNTIME)/ptc_console_input.c $(RUNTIME)/ptc_console_output.c $(RUNTIME)/ptc_console_oric.c

console.pre.c: console.c $(RUNTIME)/ptc_console.h
	$(HOSTCPP) -P -nostdinc -I$(RUNTIME) $< > $@
console_input.pre.c: $(RUNTIME)/ptc_console_input.c $(RUNTIME)/ptc_console.h
	$(HOSTCPP) -P -nostdinc -I$(RUNTIME) $< > $@
console_output.pre.c: $(RUNTIME)/ptc_console_output.c $(RUNTIME)/ptc_console.h
	$(HOSTCPP) -P -nostdinc -I$(RUNTIME) $< > $@
console_adapter.pre.c: $(RUNTIME)/ptc_console_oric.c $(RUNTIME)/ptc_console.h
	test -f "$(OSDK)/include/stdio.h"
	$(HOSTCPP) -P -nostdinc -I$(RUNTIME) -I"$(OSDK)/include" -DPTC_OSDK -D__16BIT__ -D__NOFLOAT__ -DATMOS $< > $@

%.c2: %.pre.c
	$(LCC65) -N$* $< > $@
%.s: %.c2
	test -f "$(OSDKMACROS)"
	macrosplitter -M"$(OSDKMACROS)" $< $@

linked.s: console.s console_input.s console_output.s console_adapter.s
	test -f "$(OSDK)/lib/library.ndx"
	link65 -d "$(OSDK)/lib/" -i "$(OSDK)/lib/" -o $@ -f -q console.s console_input.s console_output.s console_adapter.s
console-osdk.bin: linked.s
	xa -I"$(OSDK)/lib" $< -o $@ -bt $(OSDKADDR) -DASSEMBLER=XA -W -C
console-osdk.tap: console-osdk.bin
	header -nCONSOLE -a1 $< $@ $(OSDKADDR)

clean:
	rm -f console-demo console-host.c console.c *.pre.c *.c2 console.s console_input.s console_output.s console_adapter.s linked.s *.bin *.tap *.o
