CC      ?= cc
CFLAGS  ?= -std=c89

.PHONY: all bootstrap test clean

all:
	$(CC) $(CFLAGS) -x c -o ptc - < generated/ptc.c
	./ptc < modified/ptc.p > ptc-new.c
	cmp generated/ptc.c ptc-new.c
	$(CC) $(CFLAGS) -x c -o ptc-new - < ptc-new.c
	cmp ptc ptc-new
	md5sum generated/ptc.c ptc-new.c
	md5sum ptc ptc-new

bootstrap:
	$(CC) $(CFLAGS) -x c -o ptc-new - < generated/ptc.c

test: ptc-new
	./ptc-new < test.pas > test.c
	$(CC) $(CFLAGS) -o test test.c
	./test

clean:
	rm -f ptc ptc-new ptc-new.c test test.c
