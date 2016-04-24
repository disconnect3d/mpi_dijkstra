#!/bin/bash
# todo: change to [cq]make
#
# The dynamic linker can't find OR finds wrong version of libstdc++-6.dll
# The easiest way to fix it is to compile with -static instead.

CXX=g++
CXXFLAGS=-std=c++14 -Wall -Wextra -static
DBGFLAGS=-ggdb -DDEBUG -DDEBUG_DIJKSTRA

FILES=main.cpp map.cpp dijkstra.cpp

all: main.cpp map.cpp dijkstra.cpp
	$(CXX) $(CXXFLAGS) $(FILES) -o main

dbg: main.cpp
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(FILES) -o main


test0: main
	./main testcases/testcase0

test1: main
	./main testcases/testcase1

tests:
	./main testcases/testcase0
	./main testcases/testcase1

