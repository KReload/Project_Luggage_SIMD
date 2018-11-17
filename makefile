

#Makefile
EXEC = ./test/testsd ./test/test_morpho ./test/test_mouvement ./test/test_mouvement_SSE2 ./test/test_morpho_SSE2 ./test/test_macros_SSE2 ./benchmark/bench ./benchmark/bench_SSE2
HDIR = ./output/hallSSE/FDE/* ./output/hallSSE/FDO/* ./output/hallSSE/SDE/* ./output/hallSSE/SDO/* ./output/hallScalar/FDE/* ./output/hallScalar/FDO/* ./output/hallScalar/SDE/* ./output/hallScalar/SDO/*
OBJ = ./obj/*.o
OTHER = ./test/*.c~ ./test/*.pgm ./include/*.c~ ./include/*.h~ *.c~
LIB = -lm

test: nrutil.o vnrutil.o simdutil.o morpho.o mouvement.o mouvement_SSE2.o morpho_SSE2.o testutil.o
	gcc test/test_morpho.c ./obj/nrutil.o ./obj/morpho.o ./obj/testutil.o -o test/test_morpho
	gcc test/test_macros_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/testutil.o -Wall -o test/test_macros_SSE2
	gcc test/test_morpho_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/mouvement_SSE2.o ./obj/morpho_SSE2.o ./obj/testutil.o -Wall -o test/test_morpho_SSE2
	gcc test/test_mouvement_SSE2.c ./obj/vnrutil.o ./obj/simdutil.o ./obj/nrutil.o ./obj/testutil.o -Wall -o test/test_mouvement_SSE2
	gcc test/test_mouvement.c ./obj/nrutil.o ./obj/testutil.o ./obj/mouvement.o -Wall -o test/test_mouvement

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

