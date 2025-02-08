CC = g++
CFLAGS = -O3 --std=c++14

all: main.exe
main.exe : main.o popolazione.o individuo.o cities.o random.o
	$(CC) main.o popolazione.o individuo.o cities.o random.o -o main.exe $(CFLAGS)
main.o : main.cpp popolazione.h individuo.h cities.h random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
popolazione.o : popolazione.cpp popolazione.h random.h individuo.h cities.h
	$(CC) -c popolazione.cpp -o popolazione.o $(CFLAGS)
individuo.o : individuo.cpp individuo.h popolazione.h cities.h random.h
	$(CC) -c individuo.cpp -o individuo.o $(CFLAGS)
cities.o : cities.cpp cities.h popolazione.h individuo.h random.h
	$(CC) -c cities.cpp -o cities.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main.exe
remove:
	rm *.txt
