CC=clang
CFLAGS=-O2 -mavx


all: twinprimetest


twinprimetest: twinprimetest.c sieve.c
	$(CC) $(CFLAGS) twinprimetest.c sieve.c -o twinprimetest -lm


#sieve: sieve.c
#	$(CC) $(CFLAGS) sieve.c -o sieve -lm



#table_dp.h: mktable.py
#	./mktable.py table_dp.h

