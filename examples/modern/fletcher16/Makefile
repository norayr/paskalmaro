CC       ?= cc
CFLAGS   ?= -std=c89 -O2
PTC      ?= ../../../ptc
PTCFLAGS ?= -r

.PHONY: all clean
all: fletcher16
fletcher16: checksum.c input.c
	$(CC) $(CFLAGS) -o $@ checksum.c input.c
checksum.c: checksum.p $(PTC)
	$(PTC) $(PTCFLAGS) < $< > $@
clean:
	rm -f fletcher16 checksum.c
