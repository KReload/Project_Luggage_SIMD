

#Makefile
all: nrutil.o
	gcc sigdelta.c ./exe/nrutil.o -o sigdelta -lm
	gcc testOpen.c ./exe/nrutil.o -o testOpen

nrutil.o:
	gcc -c ./include/nrutil.c -o ./exe/nrutil.o
