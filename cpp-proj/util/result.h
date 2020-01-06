#ifndef CPP_PROJ_RESULT_H
#define CPP_PROJ_RESULT_H
#include <chrono>
#include <vector>

struct result {
    unsigned int clauses_satisfied;
    std::vector<bool> solution;
    long time;
};

#endif //CPP_PROJ_RESULT_H
