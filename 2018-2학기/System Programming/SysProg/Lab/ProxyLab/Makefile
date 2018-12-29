##############################################################################
# DON'T NEED TO MODIFY
##############################################################################

CC=gcc -m32
CFLAGS=-Wall
LIBS=-lpthread

HTTP=http
PROXY=proxy

FILES = Makefile csapp.h csapp.c cache.h cache.h $(PROXY).c $(HTTP).c

PROGS = proxy http

all: $(PROGS)

proxy: $(PROXY).c csapp.o cache.o csapp.h cache.h
	$(CC) $(CFLAGS) $(LIBS) -o proxy $(PROXY).c csapp.o cache.o

http: $(HTTP).c csapp.o csapp.h
	$(CC) $(CFLAGS) $(LIBS) -o http $(HTTP).c csapp.o

cache.o: cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

clean:
	rm -f *.o *~ *.tar
	rm -f $(PROGS)
