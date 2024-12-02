// satsolver.cc
#include "satsolver.h"

SATSolver::SATSolver(const CNF& cnf, int numVars)
    : clauses(cnf), assignment(numVars + 1, 0), occurrences(numVars + 1, std::vector<int>()), numVariables(numVars) {
    // Build occurrences: for each variable, store the indices of clauses it appears in
    for (size_t i = 0; i < clauses.size(); ++i) {
        for (auto lit : clauses[i]) {
            int var = abs(lit);
            if (var <= numVariables) {
                occurrences[var].push_back(i);
            }
        }
    }
}

bool SATSolver::solve() {
    return dpll();
}

bool SATSolver::dpll() {
    // Perform unit propagation
    if (!unitPropagation()) {
        return false;
    }

    // Check if all variables are assigned
    bool allAssigned = true;
    for (int var = 1; var <= numVariables; ++var) {
        if (assignment[var] == 0) {
            allAssigned = false;
            break;
        }
    }
    if (allAssigned) {
        return true; // All variables assigned without conflict
    }

    // Select an unassigned variable using a simple heuristic (most frequent in clauses)
    int var = selectUnassignedVariable();
    if (var == -1) {
        return false; // No variable found (should not happen)
    }

    // Try assigning true
    assignment[var] = 1;
    if (dpll()) {
        return true;
    }

    // Backtrack and try assigning false
    assignment[var] = -1;
    if (dpll()) {
        return true;
    }

    // Backtrack
    assignment[var] = 0;
    return false;
}

bool SATSolver::unitPropagation() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& clause : clauses) {
            int unassigned = 0;
            int lastLit = 0;
            bool clauseSatisfied = false;
            for (auto lit : clause) {
                int var = abs(lit);
                if (assignment[var] == (lit > 0 ? 1 : -1)) {
                    clauseSatisfied = true;
                    break;
                }
                if (assignment[var] == 0) {
                    unassigned++;
                    lastLit = lit;
                }
            }
            if (!clauseSatisfied) {
                if (unassigned == 0) {
                    return false; // Conflict detected
                }
                if (unassigned == 1) {
                    // Unit clause found, assign the remaining literal
                    int var = abs(lastLit);
                    int value = (lastLit > 0) ? 1 : -1;
                    if (assignment[var] == 0) {
                        assignment[var] = value;
                        changed = true;
                    }
                }
            }
        }
    }
    return true;
}

int SATSolver::selectUnassignedVariable() {
    // Simple heuristic: select the variable that appears most frequently in clauses
    std::vector<int> frequency(numVariables + 1, 0);
    for (size_t i = 0; i < clauses.size(); ++i) {
        for (auto lit : clauses[i]) {
            int var = abs(lit);
            if (assignment[var] == 0) {
                frequency[var]++;
            }
        }
    }

    int maxFreq = -1;
    int selectedVar = -1;
    for (int var = 1; var <= numVariables; ++var) {
        if (assignment[var] == 0 && frequency[var] > maxFreq) {
            maxFreq = frequency[var];
            selectedVar = var;
        }
    }
    return selectedVar;
}

const std::vector<int>& SATSolver::getAssignment() const {
    return assignment;
}

void SATSolver::printSolution() const {
    std::cout << "SAT Solver Solution:\n";
    for (int var = 1; var <= numVariables; ++var) {
        if (assignment[var] == 1) {
            std::cout << "Variable " << var << " = TRUE\n";
        } else if (assignment[var] == -1) {
            std::cout << "Variable " << var << " = FALSE\n";
        } else {
            std::cout << "Variable " << var << " = UNASSIGNED\n";
        }
    }
}

