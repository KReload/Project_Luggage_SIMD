

#Makefile
EXEC = ./test/testsd ./test/testMorpho ./test/testOpen ./test/testfd ./exe/mouvement_SD ./exe/mouvement_FD ./exe/mouvement_SSE2 ./validation/validation_FD ./validation/validation_SD
HDIR = ./exe/hallFDE/* ./exe/hallFDO/* ./exe/hallSDO/* ./exe/hallSDE/*
OBJ = ./obj/*.o
OTHER = ./test/*.c~ ./test/*.pgm ./include/*.c~ ./include/*.h~ *.c~
LIB = -lm

all: nrutil.o vnrutil.o morpho.o framediff.o sigdelta.o simdutil.o morpho_SSE2.o
	gcc mouvement_FD.c ./obj/nrutil.o ./obj/morpho.o ./obj/framediff.o -o ./exe/mouvement_FD
	gcc mouvement_SD.c ./obj/nrutil.o ./obj/morpho.o ./obj/sigdelta.o -o ./exe/mouvement_SD
	gcc mouvement_SSE2.c ./obj/simdutil.o ./obj/vnrutil.o ./obj/nrutil.o -o ./exe/mouvement_SSE2

test: nrutil.o morpho.o framediff.o sigdelta.o
	gcc test/testOpen.c ./obj/nrutil.o -o test/testOpen
	gcc test/testMorpho.c ./obj/nrutil.o ./obj/morpho.o -o test/testMorpho
	gcc test/testfd.c ./obj/nrutil.o ./obj/morpho.o ./obj/framediff.o -o test/testfd
	gcc test/testsd.c ./obj/nrutil.o ./obj/morpho.o ./obj/sigdelta.o -o test/testsd
	gcc test/testsd_simd.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o -Wall -o test/testsd_simd

validation: nrutil.o validation.o
	gcc ./validation/validation_FD.c ./obj/nrutil.o ./obj/validation.o -o ./validation/validation_FD $(LIB)
	gcc ./validation/validation_SD.c ./obj/nrutil.o ./obj/validation.o -o ./validation/validation_SD $(LIB)

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

validation.o:
	gcc -c ./include/validation.c -o ./obj/validation.o $(LIB)

morpho_SSE2.o:
	gcc -c ./include/morpho_SSE2.c -o ./obj/morpho_SSE2.o

clean:
	rm -rf $(OBJ) $(EXEC) $(OTHER) $(HDIR)

.PHONY: bench
bench:
	gcc ./benchmark/bench.c -o ./benchmark/bench

