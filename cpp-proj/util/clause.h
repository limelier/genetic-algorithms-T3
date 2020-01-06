#ifndef CPP_PROJ_CLAUSE_H
#define CPP_PROJ_CLAUSE_H

#include <vector>
#include <string>
#include <fstream>
#include "bitset.h"

typedef std::vector<int> clause;
struct problem {
    unsigned int clauses_no = 0;
    unsigned int literals_no = 0;
    std::vector<clause> clauses;
};

problem readInput(const std::string &filename);
unsigned int evalIndividual(const problem &problem, const bitset &individual);
bool evalIndividualOnClause(const clause &clause, const bitset &individual);

#endif //CPP_PROJ_CLAUSE_H
