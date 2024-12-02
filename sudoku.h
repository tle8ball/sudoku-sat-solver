// sudoku.h
#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <memory>
#include <array>
#include <random>
#include <algorithm>

class Sudoku {
public:
    Sudoku();
    void generate();
    void print() const;
    void createPuzzle(int cellsToRemove); // Optional: Remove cells to create a puzzle

    // Getter for a specific cell's value (const)
    int getCellValue(int row, int col) const;

    // Getter for a specific cell's pointer (non-const)
    int* getCell(int row, int col);

private:
    // 9x9 grid represented as a 2D array of unique_ptr<int>
    std::array<std::array<std::unique_ptr<int>, 9>, 9> grid;

    bool fillGrid(int row, int col);
    bool isSafe(int row, int col, int num) const;
    std::array<int, 9> shuffledNumbers() const;
};

#endif // SUDOKU_H

