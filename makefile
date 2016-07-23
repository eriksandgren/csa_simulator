CC=g++
CFLAGS=-c -Wall

all: Simulator

Simulator: Simulator.o Arrivals.o Decoder.o Encoder.o Node.o
	$(CC) Simulator.o Arrivals.o Decoder.o Encoder.o Node.o -o Simulator 

Simulator.o: Simulator.cpp
	$(CC) $(CFLAGS) Simulator.cpp

Arrivals.o: Arrivals.cpp
	$(CC) $(CFLAGS) Arrivals.cpp

Decoder.o: Decoder.cpp
	$(CC) $(CFLAGS) Decoder.cpp
    
Encoder.o: Encoder.cpp
	$(CC) $(CFLAGS) Encoder.cpp

Node.o: Node.cpp
	$(CC) $(CFLAGS) Node.cpp

clean:
	rm *o Simulator