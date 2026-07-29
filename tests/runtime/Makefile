CC      ?= cc
CFLAGS  ?= -std=c89
PTC     ?= ../../ptc

TESTS   = 01_nil_deref 02_nil_arrow 03_array_bounds_hi 04_array_bounds_lo \
          06_confarr_bounds
VALID   = 05_valid

.PHONY: all clean

all: $(addsuffix .pass, $(TESTS)) $(addsuffix .pass, $(VALID))

# Tests expected to exit non-zero and print a Fatal message to stderr
$(addsuffix .pass, $(TESTS)): %.pass: %.p
	$(PTC) -r < $< > $*.c
	$(CC) $(CFLAGS) -o $* $*.c
	@if ./$* 2>&1 | grep -q 'Fatal:'; then \
		echo "PASS: $<"; \
	else \
		echo "FAIL: $< (expected Fatal message)"; exit 1; \
	fi
	@rm -f $* $*.c

# Tests expected to succeed (exit 0)
$(addsuffix .pass, $(VALID)): %.pass: %.p
	$(PTC) -r < $< > $*.c
	$(CC) $(CFLAGS) -o $* $*.c
	@if ./$* > /dev/null 2>&1; then \
		echo "PASS: $<"; \
	else \
		echo "FAIL: $< (expected clean exit)"; exit 1; \
	fi
	@rm -f $* $*.c

clean:
	rm -f *.c $(TESTS) $(VALID)
