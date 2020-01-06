#include <future>
#include "testRunner.h"
#include "methods.h"
#include "../util/clause.h"
#include "../config.h"
#include <fstream>


void runTests() {
    std::vector<std::pair<std::string, testMethod>> methodPairs = {
            {"genetic", geneticSearch},
//            {"random",  randomGeneration}
    };

    std::vector<std::string> inputFilenames{
            "1.cnf",
            "2.cnf",
            "3.cnf",
            "4.cnf",
            "5.cnf",
            "6.cnf",
            "7.cnf",
            "8.cnf",
            "9.cnf",
            "10.cnf"
    };

    for (const auto &inputFilename : inputFilenames) {
        for (const auto &methodPair : methodPairs) {
            std::string outputFilename =
                    "output/" + inputFilename + '_' + methodPair.first + ".txt";
            runTest(inputFilename, methodPair.second, outputFilename);
        }
    }
}

void runTest(const std::string &inputFilename,
             const testMethod &method,
             const std::string &outputFilename) {

    std::vector<std::future<result>> futures;
    futures.reserve(ALGORITHM_RUNS);

    auto problem = readInput(inputFilename);
    for (size_t i = 0; i < ALGORITHM_RUNS; i++) {
        futures.push_back(std::async(method, problem));
    }

    std::ofstream fout(outputFilename, std::ostream::ate | std::ostream::out);

    fout << "= OUTPUT START =" << std::endl;

    for (size_t i = 0; i < ALGORITHM_RUNS; i++) {
        auto result = futures[i].get();
        fout << result.clauses_satisfied << ' ' << result.time << ' ';
        for (size_t j = 0; j < result.solution.size(); j++) {
            if (result.solution[j]) {
                fout << j + 1 << ' ';
            }
        }
        fout << std::endl;
    }

    fout.close();
}