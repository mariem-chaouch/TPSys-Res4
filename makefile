CC = gcc
CFLAGS = -Wall -Wextra -g -D_FILE_OFFSET_BITS=64

AR = ar
ARFLAGS = rcs

all: segclient

my_libseg.o: my_libseg.c segdef.h
	$(CC) $(CFLAGS) -c my_libseg.c -o my_libseg.o

my_libseg.a: my_libseg.o
	$(AR) $(ARFLAGS) my_libseg.a my_libseg.o

segclient: segclient.c segdef.h my_libseg.a
	$(CC) $(CFLAGS) segclient.c my_libseg.a -o segclient

.INTERMEDIATE: my_libseg.o

clean:
	rm -f segclient my_libseg.o my_libseg.a

.PHONY: all run clean