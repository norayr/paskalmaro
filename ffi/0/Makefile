

all:
		../../ptc < externaltest.p > externaltest.c

		cc -std=c89 \
		-o externaltest \
		externaltest.c twice.c

		./externaltest
