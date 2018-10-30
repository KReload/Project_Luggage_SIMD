

#Makefile
all: nrutil.o
	gcc sigdelta.c nrutil.o -o sigdelta -lm
	gcc testOpen.c nrutil.o -o testOpen

nrutil.o:
	gcc -c nrutil.c -o nrutil.o
