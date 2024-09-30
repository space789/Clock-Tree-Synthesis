CC = g++
CFLAGS = -std=c++11 -O2 -g

OBJS = main.o Point.o Node.o Tree.o Grid.o BFS.o LineSegment.o

all: cts

cts: $(OBJS)
	$(CC) $(CFLAGS) -o cts $(OBJS)

main.o: main.cpp Point.h Node.h Tree.h Grid.h BFS.h LineSegment.h Utilities.h Constants.h
	$(CC) $(CFLAGS) -c main.cpp

Point.o: Point.cpp Point.h
	$(CC) $(CFLAGS) -c Point.cpp

Node.o: Node.cpp Node.h
	$(CC) $(CFLAGS) -c Node.cpp

Tree.o: Tree.cpp Tree.h Node.h Point.h Utilities.h
	$(CC) $(CFLAGS) -c Tree.cpp

Grid.o: Grid.cpp Grid.h Point.h Constants.h
	$(CC) $(CFLAGS) -c Grid.cpp

BFS.o: BFS.cpp BFS.h Grid.h Point.h Constants.h
	$(CC) $(CFLAGS) -c BFS.cpp

LineSegment.o: LineSegment.cpp LineSegment.h Point.h
	$(CC) $(CFLAGS) -c LineSegment.cpp

clean:
	rm -f *.o cts
