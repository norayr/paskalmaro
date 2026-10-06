CC       ?= cc
CFLAGS   ?= -std=c89 -O2
PTC      ?= ../../../ptc
PTCFLAGS ?= -r

.PHONY: all clean
all: textstats
textstats: textstats.c ../fletcher16/input.c
	$(CC) $(CFLAGS) -o $@ textstats.c ../fletcher16/input.c
textstats.c: textstats.p $(PTC)
	$(PTC) $(PTCFLAGS) < $< > $@
clean:
	rm -f textstats textstats.c
