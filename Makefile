CC=clang
CFLAGS=-O2 -mavx

twinprimetest: twinprimetest.c table_dp.h
	$(CC) $(CFLAGS) twinprimetest.c -o twinprimetest



table_dp.h: mktable.py
	./mktable.py table_dp.h
