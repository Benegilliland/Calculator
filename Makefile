CXX = g++
CPPFLAGS = -O2 -march=native

build/calc: src/main.cpp
	$(CXX) $(CPPFLAGS) -o build/calc src/main.cpp

.PHONY: clean run

clean:
	rm build/calc

run: build/calc
	build/calc
