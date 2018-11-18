

#Makefile
EXEC = ./test/test_morpho ./test/test_mouvement ./test/test_mouvement_SSE2 ./test/test_morpho_SSE2 ./test/test_macros_SSE2 ./test/test_IVT ./exe/*
HDIR = ./output/hallSSE/FDE/* ./output/hallSSE/FDO/* ./output/hallSSE/SDE/* ./output/hallSSE/SDO/* ./output/hallScalar/FDE/* ./output/hallScalar/FDO/* ./output/hallScalar/SDE/* ./output/hallScalar/SDO/* ./output/hallOMP/FDO/* ./output/hallOMP/FDE/* ./output/hallOMP/SDO/* ./output/hallOMP/SDE/* ./output/hallOMPxSSE2/FDO/* ./output/hallOMPxSSE2/FDE/* ./output/hallOMPxSSE2/SDO/* ./output/hallOMPxSSE2/SDE/*
OBJ = ./obj/*.o
OTHER = ./test/*.c~ ./test/*.pgm ./include/*.c~ ./include/*.h~ *.c~ ./src/*.c~
LIB = -lm
OMP = -fopenmp

test: nrutil.o vnrutil.o simdutil.o morpho.o mouvement.o mouvement_SSE2.o morpho_SSE2.o testutil.o
	gcc test/test_morpho.c ./obj/nrutil.o ./obj/morpho.o ./obj/testutil.o -o test/test_morpho
	gcc test/test_macros_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/testutil.o -Wall -o test/test_macros_SSE2
	gcc test/test_morpho_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/testutil.o -Wall -o test/test_morpho_SSE2
	gcc test/test_mouvement_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/testutil.o -Wall -o test/test_mouvement_SSE2
	gcc test/test_mouvement.c ./obj/nrutil.o ./obj/testutil.o ./obj/mouvement.o -Wall -o test/test_mouvement
	gcc ./test/test_IVT.c ./obj/nrutil.o -o ./test/test_IVT

mouvement_SSE2.o:
	gcc -c ./src/mouvement_SSE2.c -o ./obj/mouvement_SSE2.o

mouvement_OMP.o:
	gcc -c ./src/mouvement_OMP.c -o ./obj/mouvement_OMP.o $(OMP)

mouvement_OMPxSSE2.o:
	gcc -c ./src/mouvement_OMPxSSE2.c -o ./obj/mouvement_OMPxSSE2.o $(OMP)

nrutil.o:
	gcc -c ./src/nrutil.c -o ./obj/nrutil.o

vnrutil.o:
	gcc -c ./src/vnrutil.c -o ./obj/vnrutil.o

morpho.o:
	gcc -c ./src/morpho.c -o ./obj/morpho.o

mouvement.o:
	gcc -c ./src/mouvement.c -o ./obj/mouvement.o

simdutil.o:
	gcc -c ./src/simdutil.c -o ./obj/simdutil.o

testutil.o:
	gcc -c ./src/testutil.c -o ./obj/testutil.o

morpho_SSE2.o:
	gcc -c ./src/morpho_SSE2.c -o ./obj/morpho_SSE2.o

morpho_OMP.o:
	gcc -c ./src/morpho_OMP.c -o ./obj/morpho_OMP.o $(OMP)

morpho_OMPxSSE2.o:
	gcc -c ./src/morpho_OMPxSSE2.c -o ./obj/morpho_OMPxSSE2.o $(OMP)

clean:
	rm -rf $(OBJ) $(EXEC) $(OTHER) $(HDIR)

.PHONY: bench
bench: nrutil.o morpho.o mouvement.o
	gcc ./src/bench.c ./obj/nrutil.o ./obj/morpho.o ./obj/mouvement.o -o ./exe/bench $(LIB)
bench_SSE2: mouvement_SSE2.o vnrutil.o simdutil.o nrutil.o morpho_SSE2.o
	gcc ./src/bench_SSE2.c ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o -o ./exe/bench_SSE2 $(LIB)
bench_OMP: nrutil.o morpho_OMP.o mouvement_OMP.o
	gcc ./src/bench_OMP.c ./obj/nrutil.o ./obj/morpho_OMP.o ./obj/mouvement_OMP.o -o ./exe/bench_OMP $(OMP) $(LIB)
bench_OMPxSSE2: mouvement_OMPxSSE2.o vnrutil.o simdutil.o nrutil.o morpho_OMPxSSE2.o
	gcc ./src/bench_OMPxSSE2.c ./obj/mouvement_OMPxSSE2.o ./obj/morpho_OMPxSSE2.o ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o -o ./exe/bench_OMPxSSE2 $(OMP) $(LIB)

