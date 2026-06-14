
bootstrap:

		cc -std=c89 -o ptc-new generated/ptc.c

all:
#		fpc -Miso ptc.pas
		cc -std=c89 -o ptc generated/ptc.c
		./ptc < ptc.p > ptc-new.c
		cc -std=c89 -o ptc-new ptc-new.c
		md5sum ptc-new.c generated/ptc.c
		md5sum ptc ptc-new

test:
		./ptc-new < test.pas > test.c
		cc -std=gnu89 -o test test.c
		./test

