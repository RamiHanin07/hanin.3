CC=g++
CFLAGS= -o

main: Coordinator.cpp
	$(CC) $(CFLAGS) Coordinator Coordinator.cpp

palin : palin.cpp
	$(CC) $(CFLAGS) palin palin.cpp
	
clean:
	rm main.o bin_adder.o