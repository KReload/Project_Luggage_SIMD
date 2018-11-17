

#Makefile
EXEC = ./test/testsd ./test/testMorpho ./test/testOpen ./test/testfd ./exe/mouvement_SD ./exe/mouvement_FD ./exe/mouvement_SSE2 ./validation/validation_FD ./validation/validation_SD ./benchmark/bench ./benchmark/bench_SSE2
HDIR = ./output/hallSSE/FDE/* ./output/hallSSE/FDO/* ./output/hallSSE/SDE/* ./output/hallSSE/SDO/* ./output/hallScalar/FDE/* ./output/hallScalar/FDO/* ./output/hallScalar/SDE/* ./output/hallScalar/SDO/*
OBJ = ./obj/*.o
OTHER = ./test/*.c~ ./test/*.pgm ./include/*.c~ ./include/*.h~ *.c~
LIB = -lm

all: nrutil.o vnrutil.o morpho.o mouvement.o simdutil.o morpho_SSE2.o mouvement_SSE2.o
	gcc mouvement_FD.c ./obj/nrutil.o ./obj/morpho.o ./obj/mouvement.o -o ./exe/mouvement_FD
	gcc mouvement_SD.c ./obj/nrutil.o ./obj/morpho.o ./obj/mouvement.o -o ./exe/mouvement_SD

test: nrutil.o vnrutil.o simdutil.o morpho.o mouvement.o mouvement_SSE2.o morpho_SSE2.o testutil.o
	gcc test/testMorpho.c ./obj/nrutil.o ./obj/morpho.o -o test/testMorpho
	gcc test/testfd.c ./obj/nrutil.o ./obj/morpho.o ./obj/mouvement.o -o test/testfd
	gcc test/testsd.c ./obj/nrutil.o ./obj/morpho.o ./obj/mouvement.o -o test/testsd
	gcc test/test_macros_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/testutil.o -Wall -o test/test_macros_SSE2
	gcc test/test_morpho_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o -Wall -o test/test_morpho_SSE2
	gcc test/test_mouvement_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/testutil.o -Wall -o test/test_mouvement_SSE2

mouvement_SSE2.o:
	gcc -c ./mouvement_SSE2.c -o ./obj/mouvement_SSE2.o

nrutil.o:
	gcc -c ./include/nrutil.c -o ./obj/nrutil.o

vnrutil.o:
	gcc -c ./include/vnrutil.c -o ./obj/vnrutil.o

morpho.o:
	gcc -c ./include/morpho.c -o ./obj/morpho.o

mouvement.o:
	gcc -c mouvement.c -o ./obj/mouvement.o

simdutil.o:
	gcc -c ./include/simdutil.c -o ./obj/simdutil.o

testutil.o:
	gcc -c ./include/testutil.c -o ./obj/testutil.o

validation.o:
	gcc -c ./include/validation.c -o ./obj/validation.o $(LIB)

morpho_SSE2.o:
	gcc -c ./include/morpho_SSE2.c -o ./obj/morpho_SSE2.o

clean:
	rm -rf $(OBJ) $(EXEC) $(OTHER) $(HDIR)

.PHONY: bench
bench: nrutil.o morpho.o mouvement.o
	gcc ./benchmark/bench.c ./obj/nrutil.o ./obj/morpho.o ./obj/mouvement.o -o ./benchmark/bench $(LIB)
bench_SSE2: mouvement_SSE2.o vnrutil.o simdutil.o nrutil.o morpho_SSE2.o
	gcc ./benchmark/bench_SSE2.c ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o -o ./benchmark/bench_SSE2 $(LIB)

