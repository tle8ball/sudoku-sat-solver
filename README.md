# Sudoku SAT Solver in C++20

## Overview

This project consists of a Sudoku puzzle generator and solver implemented in C++20. It utilizes a custom DPLL-based SAT solver to solve Sudoku puzzles by encoding them into CNF (Conjunctive Normal Form).

## Features

- **Generate Sudoku Grids:** Creates complete and valid Sudoku puzzles.
- **Create Sudoku Puzzles:** Removes a specified number of cells to form playable puzzles.
- **Solve Sudoku Puzzles:** Uses a SAT solver to find solutions to Sudoku puzzles.

## Requirements

- **C++20 Compiler:** Ensure you have a C++20 compatible compiler like `g++` or `clang++`.
- **Make (Optional):** For using the provided Makefile.

## Installation

### Using Makefile (Recommended)

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/sudoku-satsolver.git
   cd sudoku-satsolver
   ```

2. **Build the Project:**
   ```bash
   make
   ```

3. **Run the Solver:**
   ```bash
   ./sudoku_solver
   ```

4. **Clean Build Files:**
   ```bash
   make clean
   ```

### Manual Compilation
If you prefer not to use the Makefile, compile the project manually:
   ```bash
   g++ -std=c++20 -Wall -Wextra -O2 -o sudoku_solver main.cc sudoku.cc satsolver.cc
   ```

## Usage
After building the project, execute the solver:
   ```bash
   ./sudoku_solver
   ```

The program will
1. Generate a complete Sudoku grid.
2. Create a puzzle by removing a specified number of cells.
3. Solve the puzzle using the SAT solver.
4. Display the solved Sudoku grid.

## Example Output

```
Generated Complete Sudoku Grid:
7 5 3 |1 6 9 |8 4 2 
1 2 9 |8 4 3 |6 7 5 
8 6 4 |2 7 5 |9 1 3 
------+-------+------
5 8 1 |4 3 7 |2 6 9 
6 4 2 |5 9 8 |1 3 7 
3 9 7 |6 2 1 |5 8 4 
------+-------+------
9 3 8 |7 5 6 |4 2 1 
2 1 5 |3 8 4 |7 9 6 
4 7 6 |9 1 2 |3 5 8 

Sudoku Puzzle with 40 cells removed:
7 5 3 |1 6 0 |8 0 2 
1 0 9 |8 0 3 |6 7 0 
8 6 0 |2 7 5 |9 1 3 
------+-------+------
5 8 1 |0 3 7 |2 0 9 
6 4 2 |5 0 8 |1 3 7 
3 9 7 |6 2 1 |5 8 4 
------+-------+------
9 0 8 |7 5 0 |4 2 1 
2 1 5 |3 8 4 |0 9 6 
4 7 6 |9 1 2 |3 5 8 

SAT Solver found a solution!

Solved Sudoku Grid:
7 5 3 |1 6 9 |8 4 2 
1 2 9 |8 4 3 |6 7 5 
8 6 4 |2 7 5 |9 1 3 
------+-------+------
5 8 1 |4 3 7 |2 6 9 
6 4 2 |5 9 8 |1 3 7 
3 9 7 |6 2 1 |5 8 4 
------+-------+------
9 3 8 |7 5 6 |4 2 1 
2 1 5 |3 8 4 |7 9 6 
4 7 6 |9 1 2 |3 5 8
```
