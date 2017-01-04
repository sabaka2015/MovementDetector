CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp FramesDifference.cpp Scenarios.cpp
PROG = program

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

program: main.o FramesDifference.o Scenarios.o
		$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

main.o: main.cpp
		$(CC) $(CFLAGS) -o main.o -c main.cpp $(LIBS)
FramesDifference.o: FramesDifference.cpp
		$(CC) $(CFLAGS) -o FramesDifference.o -c FramesDifference.cpp $(LIBS)
Scenarios.o: Scenarios.cpp
		$(CC) $(CFLAGS) -o Scenarios.o -c Scenarios.cpp $(LIBS)

clean:
		rm -f *.o
.PHONY: clean
