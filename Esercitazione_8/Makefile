CC = g++
CFLAGS = -Wall -O3 --std=c++11

Main.exe : Main.o random.o
	$(CC) random.o Main.o -o Main.exe
Main.o : Main.cpp
	$(CC) -c Main.cpp -o Main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o Main.exe seed.out
