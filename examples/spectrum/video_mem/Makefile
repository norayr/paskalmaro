ZCC     ?= zcc
ZFLAGS  ?= +zx
PTC     ?= ../../../ptc
PTCFLAGS ?= -m -i16

.PHONY: all clean

all: video.tap

video.tap: video.c io.c
	$(ZCC) $(ZFLAGS) -o video video.c io.c -create-app

video.c: video.p
	$(PTC) $(PTCFLAGS) < video.p > video.c

clean:
	rm -f video.c video.tap
