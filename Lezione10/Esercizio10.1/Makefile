CC = g++
CFLAGS = -O3 --std=c++14
MPI_INCLUDE = -I/opt/homebrew/opt/open-mpi/include
MPI_LIB = -L/opt/homebrew/opt/open-mpi/lib
MPI_LIBS = -lmpi

all: main.exe
main.exe : main.o popolazione.o individuo.o cities.o random.o
	$(CC) $(MPI_INCLUDE) $(MPI_LIB) $(MPI_LIBS) main.o popolazione.o individuo.o cities.o random.o -o main.exe $(CFLAGS)
main.o : main.cpp popolazione.h individuo.h cities.h random.h
	$(CC) $(MPI_INCLUDE) $(MPI_LIB) $(MPI_LIBS) -c main.cpp -o main.o $(CFLAGS)
popolazione.o : popolazione.cpp popolazione.h random.h individuo.h cities.h
	$(CC) $(MPI_INCLUDE) $(MPI_LIB) $(MPI_LIBS) -c popolazione.cpp -o popolazione.o $(CFLAGS)
individuo.o : individuo.cpp individuo.h popolazione.h cities.h random.h
	$(CC) $(MPI_INCLUDE) $(MPI_LIB) $(MPI_LIBS) -c individuo.cpp -o individuo.o $(CFLAGS)
cities.o : cities.cpp cities.h popolazione.h individuo.h random.h
	$(CC) $(MPI_INCLUDE) $(MPI_LIB) $(MPI_LIBS) -c cities.cpp -o cities.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) $(MPI_INCLUDE) $(MPI_LIB) $(MPI_LIBS) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main.exe
remove:
	rm *.txt
esegui: main.exe
	/opt/homebrew/opt/open-mpi/bin/mpiexec -n 8 ./main.exe
  # Esegui con 8 nodi
