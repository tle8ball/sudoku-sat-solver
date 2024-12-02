// sudoku.cc
#include "sudoku.h"

Sudoku::Sudoku() {
    for(auto & row : grid) {
        for(auto & cell : row) {
            cell = std::make_unique<int>(0);
        }
    }
}

void Sudoku::generate() {
    if (!fillGrid(0, 0)) {
        std::cerr << "Failed to generate Sudoku grid." << std::endl;
    }
}

bool Sudoku::fillGrid(int row, int col) {
    if (row == 9) {
        // Reached the end of the grid
        return true;
    }

    // Move to the next row if we reach the end of the current row
    if (col == 9) {
        return fillGrid(row + 1, 0);
    }

    // Shuffle numbers to ensure randomness
    auto numbers = shuffledNumbers();

    for(int num : numbers) {
        if (isSafe(row, col, num)) {
            *grid[row][col] = num;
            if (fillGrid(row, col + 1)) {
                return true;
            }
            // Backtrack
            *grid[row][col] = 0;
        }
    }

    return false; // Trigger backtracking
}

bool Sudoku::isSafe(int row, int col, int num) const {
    // Check row
    for(int c = 0; c < 9; ++c) {
        if(*grid[row][c] == num) return false;
    }

    // Check column
    for(int r = 0; r < 9; ++r) {
        if(*grid[r][col] == num) return false;
    }

    // Check 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for(int r = startRow; r < startRow + 3; ++r) {
        for(int c = startCol; c < startCol + 3; ++c) {
            if(*grid[r][c] == num) return false;
        }
    }

    return true;
}

std::array<int, 9> Sudoku::shuffledNumbers() const {
    std::array<int, 9> numbers = {1,2,3,4,5,6,7,8,9};
    // Use a random device and generator for shuffling
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
    return numbers;
}

void Sudoku::print() const {
    for(int r = 0; r < 9; ++r) {
        if(r % 3 == 0 && r != 0) {
            std::cout << "------+-------+------\n";
        }
        for(int c = 0; c < 9; ++c) {
            if(c % 3 == 0 && c != 0) {
                std::cout << "| ";
            }
            std::cout << *grid[r][c] << ' ';
        }
        std::cout << '\n';
    }
}

void Sudoku::createPuzzle(int cellsToRemove) {
    if(cellsToRemove < 0 || cellsToRemove > 81) {
        std::cerr << "Invalid number of cells to remove." << std::endl;
        return;
    }

    static std::random_device rd;
    static std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, 80);

    int removed = 0;
    while(removed < cellsToRemove) {
        int pos = dist(g);
        int row = pos / 9;
        int col = pos % 9;
        if(*grid[row][col] != 0) {
            *grid[row][col] = 0;
            removed++;
        }
    }
}

int Sudoku::getCellValue(int row, int col) const {
    if(row < 1 || row > 9 || col < 1 || col > 9) {
        std::cerr << "Invalid cell coordinates.\n";
        return 0;
    }
    return *grid[row - 1][col - 1];
}

int* Sudoku::getCell(int row, int col) {
    if(row < 1 || row > 9 || col < 1 || col > 9) {
        std::cerr << "Invalid cell coordinates.\n";
        return nullptr;
    }
    return grid[row - 1][col - 1].get();
}

