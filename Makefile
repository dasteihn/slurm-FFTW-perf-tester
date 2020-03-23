all:
	mpiicc -c $(FFTW_INC) mpi-thread-demo.c
	mpiicc $(FFTW_INC) -o measprog mpi-thread-demo.o $(FFTW_SHLIB) $(FFTW_MPI_LIB) -lpthread -lfftw3_threads
	cp measprog ./1-node/
	cp measprog ./2-nodes/
	cp measprog ./4-nodes/
	mkdir ./1-node/logfiles ./2-nodes/logfiles ./4-nodes/logfiles

clean:
	rm mpi-thread-demo.o measprog
	rm ./1-node/measprog
	rm ./2-nodes/measprog
	rm ./4-nodes/measprog
