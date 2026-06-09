CC = g++
CFLAGS = -Wall -O3 --std=c++11
AFLAGS = -larmadillo
Main.exe : Main.o random.o chromosome.o population.o
	$(CC) random.o Main.o chromosome.o population.o -o Main.exe $(AFLAGS)
Main.o : Main.cpp
	$(CC) -c Main.cpp -o Main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
chromosome.o : chromosome.cpp chromosome.h
	$(CC) -c chromosome.cpp -o chromosome.o $(CFLAGS)
population.o :population.cpp population.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)	
clean :
	rm *.o Main.exe
