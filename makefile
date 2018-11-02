

#Makefile
EXEC = ./test/testsd ./test/testMorpho ./test/testOpen ./test/testfd ./exe/*
OBJ = ./obj/nrutil.o ./obj/morpho.o ./obj/sigdelta.o ./obj/framediff.o
OTHER = ./test/*.c~ ./test/*.pgm ./include/*.c~ ./include/*.h~ *.c~

all: nrutil.o morpho.o framediff.o sigdelta.o
	gcc mouvement_FD.c ./obj/nrutil.o ./obj/morpho.o ./obj/framediff.o -o ./exe/mouvement_FD

test: nrutil.o morpho.o framediff.o sigdelta.o
	gcc test/testOpen.c ./obj/nrutil.o -o test/testOpen
	gcc test/testMorpho.c ./obj/nrutil.o ./obj/morpho.o -o test/testMorpho
	gcc test/testfd.c ./obj/nrutil.o ./obj/morpho.o ./obj/framediff.o -o test/testfd
	gcc test/testsd.c ./obj/nrutil.o ./obj/morpho.o ./obj/sigdelta.o -o test/testsd

nrutil.o:
	gcc -c ./include/nrutil.c -o ./obj/nrutil.o

morpho.o:
	gcc -c ./include/morpho.c ./obj/nrutil.o -o ./obj/morpho.o

framediff.o:
	gcc -c ./include/framediff.c ./obj/nrutil.o -o ./obj/framediff.o

sigdelta.o:
	gcc -c ./include/sigdelta.c ./obj/nrutil.o -o ./obj/sigdelta.o

clean:
	rm -rf $(OBJ) $(EXEC) $(OTHER)

.PHONY: bench
bench:
	gcc ./benchmark/bench.c -o ./benchmark/bench

