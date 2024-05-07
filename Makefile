CXX = g++
CPPFLAGS = -O2 -march=native -Wall
LIBS = -lgmpxx -lgmp

build/calc: build/main.o build/operator.o build/parse.o
	$(CXX) $(CPPFLAGS) -o build/calc build/main.o build/parse.o build/operator.o $(LIBS)

build/main.o: src/main.cpp
	$(CXX) $(CPPFLAGS) -o build/main.o -c src/main.cpp $(LIBS)

build/operator.o: src/operator.cpp src/operator.h
	$(CXX) $(CPPFLAGS) -o build/operator.o -c src/operator.cpp $(LIBS)

build/parse.o: src/parse.cpp src/parse.h
	$(CXX) $(CPPFLAGS) -o build/parse.o -c src/parse.cpp $(LIBS)

.PHONY: clean run

clean:
	rm build/calc build/main.o build/operator.o build/parse.o

run: build/calc
	build/calc
