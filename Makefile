# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS= -std=c++11 -g -Wall -gdwarf-3
LDFLAGS=-g

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder

# include what ever source code *.hpp files pathfinder relies on (these are merely the ones that were used in the solution)

pathfinder: ActorGraph.o

# include what ever source code *.hpp files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.hpp file that is already included with class/method headers

Edge.o: Edge.hpp

Actor.o: Actor.hpp

ActorGraph.o: Edge.o Actor.o ActorGraph.hpp
#commented out UnionFind.hpp and Movie.hpp for now

clean:
	rm -f pathfinder *.o core*

