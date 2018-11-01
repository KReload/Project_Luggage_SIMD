

#Makefile
EXEC = sigdelta framediff testOpen testMorpho


all: nrutil.o morpho.o
	gcc sigdelta.c ./exe/nrutil.o -o sigdelta
	gcc framediff.c ./exe/nrutil.o -o framediff
	gcc testOpen.c ./exe/nrutil.o -o testOpen
	gcc testMorpho.c ./exe/*.o -o testMorpho

nrutil.o:
	gcc -c ./include/nrutil.c -o ./exe/nrutil.o
<<<<<<< Updated upstream
clean:
	rm sigdelta testOpen ./benchmark/bench ./exe/*
.PHONY: bench
bench:
	gcc ./benchmark/bench.c -o ./benchmark/bench
=======

morpho.o:
	gcc -c ./include/morpho.c ./exe/nrutil.o -o ./exe/morpho.o

clean:
	rm -rf ./exe/*.o $(EXEC)
>>>>>>> Stashed changes
