CC=gcc
CFLAGS=-c -Wall

SHELL := /bin/bash

all: http

http: main.o indexgen.o
	$(CC) main.o indexgen.o -o http

main.o: main.c
	$(CC) $(CFLAGS) main.c

indexgen.o: indexgen.c
	$(CC) $(CFLAGS) indexgen.c

clean:
	    rm *o http
