// satsolver.h
#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using Literal = int; // Positive for normal, negative for negated
using Clause = std::vector<Literal>;
using CNF = std::vector<Clause>;

class SATSolver {
public:
    SATSolver(const CNF& cnf, int numVars);
    bool solve();
    void printSolution() const;

    // Getter for the assignment vector
    const std::vector<int>& getAssignment() const;

private:
    CNF clauses;
    std::vector<int> assignment; // 0: unassigned, 1: true, -1: false
    std::vector<std::vector<int>> occurrences; // Variable to clauses mapping
    int numVariables;

    bool dpll();
    bool unitPropagation();
    int selectUnassignedVariable();
};

#endif // SATSOLVER_H

