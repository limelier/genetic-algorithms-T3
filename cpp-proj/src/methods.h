#ifndef CPP_PROJ_SRC_METHODS_H_
#define CPP_PROJ_SRC_METHODS_H_

#include <functional>
#include "../util/result.h"
#include "../util/clause.h"

result geneticSearch(const problem &problem);
result randomGeneration(const problem &problem);

typedef std::function<result(problem)> testMethod;

#endif //CPP_PROJ_SRC_METHODS_H_
