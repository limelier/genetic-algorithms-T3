#include "clause.h"

problem readInput(const std::string &filename) {
    problem problem = {0, 0, {}};
    std::ifstream fin("input/" + filename);
    char first_character;
    int clauses_scanned = 0;

    while (true) {
        first_character = fin.peek();
        if (first_character == -1) { // EOF
            break;
        }
        else if (first_character == 'c') { // comment line
            fin.ignore(1024, '\n'); // skip the line
        }
        else if (first_character == 'p') { // problem definition line
            std::string problem_type;
            fin >> first_character >> problem_type;
            if (problem_type != "cnf") {
                throw std::exception();
            }
            fin >> problem.literals_no >> problem.clauses_no;
            problem.clauses.reserve(problem.clauses_no);
        }
        else { // clause line
            clause clause;
            int num;
            fin >> num;
            while (num != 0) {
                clause.push_back(num);
                fin >> num;
            }
            problem.clauses.push_back(clause);
            clauses_scanned++;
            fin.ignore(1024, '\n');
        }
    }
    if (clauses_scanned != problem.clauses_no) {
        throw std::exception();
    }
    return problem;
}

bool isPositive(int num) {
    return num >= 0;
}

unsigned int evalIndividual(const problem &problem, const bitset &individual) {
    unsigned int clauses_satisfied = 0;
    for (const auto &clause : problem.clauses) {
        for (auto i : clause) {
            if (individual.at(abs(i) - 1) == isPositive(i)) {
                clauses_satisfied++;
                break;
            }
        }
    }
    return clauses_satisfied;
}

bool evalIndividualOnClause(const clause &clause, const bitset &individual) {
    for (auto i : clause) {
        if (individual.at(abs(i) - 1) == isPositive(i)) {
            return true;
        }
    }
    return false;
}