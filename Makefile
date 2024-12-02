# Makefile

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

# Targets
all: sudoku_solver

sudoku_solver: main.o sudoku.o satsolver.o
	$(CXX) $(CXXFLAGS) -o sudoku_solver main.o sudoku.o satsolver.o

main_solver.o: main.cc sudoku.h satsolver.h
	$(CXX) $(CXXFLAGS) -c main.cc

sudoku.o: sudoku.cc sudoku.h
	$(CXX) $(CXXFLAGS) -c sudoku.cc

satsolver.o: satsolver.cc satsolver.h
	$(CXX) $(CXXFLAGS) -c satsolver.cc

clean:
	rm -f *.o sudoku_solver

