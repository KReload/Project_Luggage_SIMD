

#Makefile
all: nrutil.o
	gcc sigdelta.c nrutil.o -o sigdelta

nrutil.o:
	gcc -c nrutil.c -o nrutil.o
