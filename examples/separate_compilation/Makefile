CC      ?= cc
CFLAGS  ?= -std=c89
PTC     ?= ../../../ptc

.PHONY: all clean

all: demo
	./demo

demo: main.c mathlib.c
	$(CC) $(CFLAGS) -o demo main.c mathlib.c

main.c: main.p
	$(PTC) < main.p > main.c

mathlib.c: mathlib.p
	$(PTC) < mathlib.p > mathlib.c

clean:
	rm -f demo main.c mathlib.c
