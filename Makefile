CC=clang
CFLAGS=-O2 -mavx

twinprimetest: twinprimetest.c table.h
	$(CC) $(CFLAGS) twinprimetest.c -o twinprimetest



table.h: mktable.py
	./mktable.py table.h
