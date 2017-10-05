CC=gcc
CFLAGS= -g -lcrypto -lrt

all: sum dump hello read

sum:	sum.c
	$(CC) $^ -o $@ $(CFLAGS)

dump:	dump.c
	$(CC) $^ -o $@ $(CFLAGS)

hello: helloWorld.c
	$(CC) $^ -o $@ $(CFLAGS)

read: readString.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f sum dump hello read
