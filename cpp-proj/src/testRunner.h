#ifndef CPP_PROJ_SRC_TESTRUNNER_H_
#define CPP_PROJ_SRC_TESTRUNNER_H_

#include <string>
#include <functional>
#include "../util/clause.h"
#include "methods.h"

void runTest(const std::string &inputFilename,
             const testMethod& method,
             const std::string &outputFilename);

void runTests();

#endif // CPP_PROJ_SRC_TESTRUNNER_H_
