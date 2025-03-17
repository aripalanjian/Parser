CXX = g++
SHELL = /bin/bash

CXXFLAGS = -g -Wall -std=c++11

HEADERS = scanner.hpp parser.hpp
OBJECTS = main.o scanner.o parser.o
TESTS = test-a1 test-a2 test-a3 test-a4 test-a5 test-a6 test-a7 test-a8

main: $(OBJECTS)
	$(CXX) -o main $(OBJECTS) 

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test-a1:
	./main a1.in

test-a2:
	./main a2.in

test-a3:
	./main a3.in

test-a4:
	./main a4.in

test-a5:
	./main a5.in

test-a6:
	./main a6.in

test-a7:
	./main a7.in

test-a8:
	./main a8.in

run: $(TESTS)

.PHONY : clean
clean:
	rm -f main $(OBJECTS)
	rm -f errReport.txt