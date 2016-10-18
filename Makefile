CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp FramesDifference.cpp
PROG = program

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

#$(PROG):$(SRCS)
#		$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
program: main.o FramesDifference.o
		$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
#program: main.o
#		$(CC) $(CFLAGS) -o $(PROG) main.o $(LIBS)
main.o: main.cpp
		$(CC) $(CFLAGS) -o main.o -c main.cpp $(LIBS)
FramesDifference.o: FramesDifference.cpp
		$(CC) $(CFLAGS) -o FramesDifference.o -c FramesDifference.cpp $(LIBS)
#CFLAGS = `pkg-config --cflags opencv`
#LIBS = `pkg-config --libs opencv`

#% : %.cpp
#		g++ $(CFLAGS) $(LIBS) -o $@ $<
#program: main.o
#		g++ $(CFLAGS) $(LIBS) -o program main.o
#main.o: main.cpp
#		g++ $(CFLAGS) $(LIBS) -o main.o -c main.cpp

