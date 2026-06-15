CC      ?= cc
CFLAGS  ?= -std=c89

.PHONY: all bootstrap update-bootstrap verify test clean

all: verify

# Build directly from the checked-in generated C source.
bootstrap:
	$(CC) $(CFLAGS) -x c -o ptc - < generated/ptc.c

# Regenerate generated/ptc.c after modifying modified/ptc.p.
update-bootstrap:
	$(CC) $(CFLAGS) -x c -o ptc-stage0 - < generated/ptc.c
	./ptc-stage0 < modified/ptc.p > ptc-stage1.c
	$(CC) $(CFLAGS) -x c -o ptc-stage1 - < ptc-stage1.c
	./ptc-stage1 < modified/ptc.p > ptc-stage2.c
	cmp ptc-stage1.c ptc-stage2.c
	cp ptc-stage1.c generated/ptc.c
	$(CC) $(CFLAGS) -x c -o ptc - < generated/ptc.c
	rm -f ptc-stage0 ptc-stage1 ptc-stage1.c ptc-stage2.c

# Verify that the checked-in C source is the fixed point of modified/ptc.p.
verify:
	$(CC) $(CFLAGS) -x c -o ptc - < generated/ptc.c
	./ptc < modified/ptc.p > ptc-new.c
	cmp generated/ptc.c ptc-new.c
	$(CC) $(CFLAGS) -x c -o ptc-new - < ptc-new.c
	cmp ptc ptc-new
	md5sum generated/ptc.c ptc-new.c
	md5sum ptc ptc-new

test: bootstrap
	./ptc < test.pas > test.c
	$(CC) $(CFLAGS) -o test test.c
	./test

clean:
	rm -f \
		ptc ptc-new ptc-new.c \
		ptc-stage0 ptc-stage1 ptc-stage1.c ptc-stage2.c \
		test test.c


