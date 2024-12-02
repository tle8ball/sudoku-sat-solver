#include "sudoku.h"
#include "satsolver.h"
#include <iostream>
#include <vector>
#include <unordered_map>

// Function to map Sudoku cell (row, col, num) to a unique variable
int getVar(int row, int col, int num) {
    return 81 * (row - 1) + 9 * (col - 1) + num;
}

// Function to encode Sudoku constraints into CNF
CNF encodeSudoku(const Sudoku& sudoku) {
    CNF cnf;

    // 1. Cell constraints: Each cell contains at least one number
    for (int row = 1; row <= 9; ++row) {
        for (int col = 1; col <= 9; ++col) {
            Clause clause;
            for (int num = 1; num <= 9; ++num) {
                clause.push_back(getVar(row, col, num));
            }
            cnf.push_back(clause);
        }
    }

    // 2. Cell constraints: Each cell contains at most one number
    for (int row = 1; row <= 9; ++row) {
        for (int col = 1; col <= 9; ++col) {
            for (int num1 = 1; num1 <= 9; ++num1) {
                for (int num2 = num1 + 1; num2 <= 9; ++num2) {
                    Clause clause;
                    clause.push_back(-getVar(row, col, num1));
                    clause.push_back(-getVar(row, col, num2));
                    cnf.push_back(clause);
                }
            }
        }
    }

    // 3. Row constraints: Each number appears exactly once in each row
    for (int row = 1; row <= 9; ++row) {
        for (int num = 1; num <= 9; ++num) {
            // At least one occurrence in the row
            Clause clause;
            for (int col = 1; col <= 9; ++col) {
                clause.push_back(getVar(row, col, num));
            }
            cnf.push_back(clause);

            // At most one occurrence in the row
            for (int col1 = 1; col1 <= 9; ++col1) {
                for (int col2 = col1 + 1; col2 <= 9; ++col2) {
                    Clause atMostOne;
                    atMostOne.push_back(-getVar(row, col1, num));
                    atMostOne.push_back(-getVar(row, col2, num));
                    cnf.push_back(atMostOne);
                }
            }
        }
    }

    // 4. Column constraints: Each number appears exactly once in each column
    for (int col = 1; col <= 9; ++col) {
        for (int num = 1; num <= 9; ++num) {
            // At least one occurrence in the column
            Clause clause;
            for (int row = 1; row <= 9; ++row) {
                clause.push_back(getVar(row, col, num));
            }
            cnf.push_back(clause);

            // At most one occurrence in the column
            for (int row1 = 1; row1 <= 9; ++row1) {
                for (int row2 = row1 + 1; row2 <= 9; ++row2) {
                    Clause atMostOne;
                    atMostOne.push_back(-getVar(row1, col, num));
                    atMostOne.push_back(-getVar(row2, col, num));
                    cnf.push_back(atMostOne);
                }
            }
        }
    }

    // 5. Block constraints: Each number appears exactly once in each 3x3 block
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int blockCol = 0; blockCol < 3; ++blockCol) {
            for (int num = 1; num <= 9; ++num) {
                // At least one occurrence in the block
                Clause clause;
                for (int row = 1; row <= 3; ++row) {
                    for (int col = 1; col <= 3; ++col) {
                        int actualRow = blockRow * 3 + row;
                        int actualCol = blockCol * 3 + col;
                        clause.push_back(getVar(actualRow, actualCol, num));
                    }
                }
                cnf.push_back(clause);

                // At most one occurrence in the block
                for (int row1 = 1; row1 <= 3; ++row1) {
                    for (int col1 = 1; col1 <= 3; ++col1) {
                        for (int row2 = row1; row2 <= 3; ++row2) {
                            for (int col2 = 1; col2 <= 3; ++col2) {
                                if (row1 == row2 && col1 >= col2)
                                    continue;
                                Clause atMostOne;
                                int actualRow1 = blockRow * 3 + row1;
                                int actualCol1 = blockCol * 3 + col1;
                                int actualRow2 = blockRow * 3 + row2;
                                int actualCol2 = blockCol * 3 + col2;
                                atMostOne.push_back(-getVar(actualRow1, actualCol1, num));
                                atMostOne.push_back(-getVar(actualRow2, actualCol2, num));
                                cnf.push_back(atMostOne);
                            }
                        }
                    }
                }
            }
        }
    }

    // 6. Pre-filled cells constraints
    for (int row = 1; row <= 9; ++row) {
        for (int col = 1; col <= 9; ++col) {
            int value = sudoku.getCellValue(row, col);
            if (value != 0) {
                Clause preFilled;
                preFilled.push_back(getVar(row, col, value));
                cnf.push_back(preFilled);
            }
        }
    }

    return cnf;
}

int main() {
    // Step 1: Generate a Sudoku puzzle
    Sudoku sudoku;
    sudoku.generate();
    std::cout << "Generated Complete Sudoku Grid:\n";
    sudoku.print();

    // Step 2: Create a puzzle by removing some cells
    int cellsToRemove = 40; // Adjust as desired
    sudoku.createPuzzle(cellsToRemove);
    std::cout << "\nSudoku Puzzle with " << cellsToRemove << " cells removed:\n";
    sudoku.print();

    // Step 3: Encode the Sudoku puzzle into CNF
    CNF cnf = encodeSudoku(sudoku);
    int numVars = 9 * 9 * 9; // 729 variables for Sudoku

    // Step 4: Initialize the SAT Solver
    SATSolver solver(cnf, numVars);

    // Step 5: Solve the SAT problem
    bool isSatisfiable = solver.solve();

    if (isSatisfiable) {
        std::cout << "\nSAT Solver found a solution!\n";
        // solver.printSolution();

        // Step 6: Update the Sudoku grid with the solution
        const std::vector<int>& assignment = solver.getAssignment();
        for (int var = 1; var <= numVars; ++var) {
            if (var > static_cast<int>(assignment.size()) - 1)
                continue;
            if (assignment[var] == 1) {
                int num = var % 9;
                if (num == 0)
                    num = 9;
                int col = ((var - 1) / 9) % 9 + 1;
                int row = (var - 1) / 81 + 1;
                int* cell = sudoku.getCell(row, col);
                if(cell != nullptr) {
                    *cell = num;
                }
            }
        }

        std::cout << "\nSolved Sudoku Grid:\n";
        sudoku.print();
    } else {
        std::cout << "\nNo solution exists for the given Sudoku puzzle.\n";
    }

    return 0;
}
