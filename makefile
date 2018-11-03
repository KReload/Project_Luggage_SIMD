

#Makefile
EXEC = ./test/testsd ./test/testMorpho ./test/testOpen ./test/testfd ./exe/*
OBJ = ./obj/nrutil.o ./obj/morpho.o ./obj/sigdelta.o ./obj/framediff.o
OTHER = ./test/*.c~ ./test/*.pgm ./include/*.c~ ./include/*.h~ *.c~

all: nrutil.o morpho.o framediff.o sigdelta.o simdutil.o
	gcc mouvement_FD.c ./obj/nrutil.o ./obj/morpho.o ./obj/framediff.o -o ./exe/mouvement_FD
	gcc mouvement_SSE2.c ./obj/simdutil.o ./obj/vnrutil.o ./obj/nrutil.o -o ./exe/mouvement_SSE2

test: nrutil.o morpho.o framediff.o sigdelta.o
	gcc test/testOpen.c ./obj/nrutil.o -o test/testOpen
	gcc test/testMorpho.c ./obj/nrutil.o ./obj/morpho.o -o test/testMorpho
	gcc test/testfd.c ./obj/nrutil.o ./obj/morpho.o ./obj/framediff.o -o test/testfd
	gcc test/testsd.c ./obj/nrutil.o ./obj/morpho.o ./obj/sigdelta.o -o test/testsd

nrutil.o:
	gcc -c ./include/nrutil.c -o ./obj/nrutil.o

vnrutil.o:
	gcc -c ./include/vnrutil.c -o ./obj/vnrutil.o

morpho.o:
	gcc -c ./include/morpho.c -o ./obj/morpho.o

framediff.o:
	gcc -c ./include/framediff.c -o ./obj/framediff.o

sigdelta.o:
	gcc -c ./include/sigdelta.c -o ./obj/sigdelta.o

simdutil.o:
	gcc -c ./include/simdutil.c -o ./obj/simdutil.o
clean:
	rm -rf $(OBJ) $(EXEC) $(OTHER)

.PHONY: bench
bench:
	gcc ./benchmark/bench.c -o ./benchmark/bench

