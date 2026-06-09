CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o Funzioni.o
	$(CC) main.o random.o Funzioni.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

Funzioni.o : Funzioni.cpp Funzioni.h
	$(CC) -c Funzioni.cpp -o Funzioni.o $(CFLAGS)
clean :
	rm *.o main.exe 
