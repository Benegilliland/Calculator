CXX = g++
CPPFLAGS = -O2 -march=native -Wall
LIBS = -lgmpxx -lgmp

build/calc: src/main.cpp
	$(CXX) $(CPPFLAGS) -o build/calc src/main.cpp $(LIBS)

.PHONY: clean run

clean:
	rm build/calc

run: build/calc
	build/calc
