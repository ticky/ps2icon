CC = cc
LD = ld

CFLAGS = -O3 -Wall
LIBS = -lm

all: init icontest

init:
	@if test ! -d objects/ ; then mkdir objects ; fi

icontest: objects/ps2icon.o objects/icontest.o
	$(CC) $(LIBS) -o icontest objects/icontest.o objects/ps2icon.o

objects/icontest.o: icontest.c
	$(CC) $(CFLAGS) -c -o objects/icontest.o icontest.c

objects/ps2icon.o: ps2icon.c ps2icon.h
	$(CC) $(CFLAGS) -c -o objects/ps2icon.o ps2icon.c

clean:
	@rm -Rf objects/ icontest
