all:
	mpiicc -Wall -Wextra -c $(FFTW_INC) measprog.c
	mpiicc $(FFTW_INC) -o measprog measprog.o $(FFTW_SHLIB) $(FFTW_MPI_LIB) -lpthread -lfftw3_threads
	cp measprog ./1-node/
	cp measprog ./2-nodes/
	cp measprog ./4-nodes/

prepare:
	mkdir ./1-node/logfiles ./2-nodes/logfiles ./4-nodes/logfiles

clean:
	rm measprog.o measprog
	rm ./1-node/measprog
	rm ./2-nodes/measprog
	rm ./4-nodes/measprog
