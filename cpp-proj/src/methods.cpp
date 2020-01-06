#include <cmath>
#include "../config.h"
#include "methods.h"
#include "../util/bitset.h"
#include "../util/random.h"
#include "../util/clause.h"

using Clock = std::chrono::high_resolution_clock;

size_t BITS;

typedef std::vector<bitset> pop;

pop generatePopulation() {
    pop population;
    population.reserve(POP_SIZE);

    auto candidate = generateBitset(BITS);

    for (int i = 0; i < POP_SIZE; i++) {
        population.push_back(generateBitset(BITS));
    }

    return population;
}

void mutate(pop &population, double multiplier) {
    for (bitset &individual : population) {
        for (auto bit : individual) {
            if (randomSubunitary() < BASE_MUTATION_CHANCE * multiplier / BITS) {
                bit = !bit;
            }
        }
    }
}

void positiveMutation(const clause &clause, bitset &individual) {
    size_t index = randomIndex(clause.size());
    int value = clause.at(index);
    individual.at(abs(value) - 1) = (value > 0);
}

void mutate_SAT(pop &population, const problem &problem) {
    for (bitset &individual : population) {
        for (auto bit : individual) {
            if (randomSubunitary() < RANDOM_MUTATION_CHANCE / BITS) {
                bit = !bit;
            }
        }
        for (size_t i = 0; i < problem.clauses_no; i++) {
            const clause &clause = problem.clauses[i];
            if (!evalIndividualOnClause(clause, individual) &&
                randomSubunitary() < POSITIVE_MUTATION_CHANCE) {
                positiveMutation(clause, individual);
            }
        }
    }
}

void crossover(pop &population) {
    shufflePopulation(population);
    int index;
    for (int i = 0; i < CROSSOVERS; i++) {
        index = 2 * i;
        auto crossoverResult = cross(population[index], population[index + 1]);
        population[index] = crossoverResult.first;
        population[index + 1] = crossoverResult.second;
    }
}

pop getElites(const pop &population, const std::vector<unsigned int> &fitness) {
    pop newPop;
    std::vector<std::pair<bitset, unsigned int>> pop_fit;
    pop_fit.reserve(population.size());
    for (size_t i = 0; i < population.size(); i++) {
        std::pair<bitset, unsigned int> pf;
        pf.first = population[i];
        pf.second = fitness[i];
        pop_fit.push_back(pf);
    }

    static auto sorter = [](const std::pair<bitset, unsigned int> &a, const std::pair<bitset, unsigned int> &b) {
        return a.second > b.second;
    };

    std::sort(pop_fit.begin(), pop_fit.end(), sorter);

    for (int i = 0; i < ELITISM_TRESHHOLD; i++) { // first few individuals are copies of the best
        newPop.push_back(pop_fit[i].first);
    }

    return newPop;
}

pop select(const pop &population, const problem &problem, unsigned int &bEval, bitset &bIndividual, double &multiplier) {
    pop newPop;
    unsigned int bCEval = 0;
    bitset bCIndividual;
    std::vector<unsigned int> eval;
    unsigned int evalSum = 0;
    eval.reserve(POP_SIZE);
    std::vector<unsigned int> fitness;
    fitness.reserve(POP_SIZE);

    for (const auto &individual : population) {
        auto value = evalIndividual(problem, individual);
        eval.push_back(value);
        if (value > bCEval) {
            bCEval = value;
            bCIndividual = individual;
        }
        evalSum += value;
    }

    if (bCEval < bEval) {
        bEval = bCEval;
        bIndividual = bCIndividual;
    }

    for (auto value : eval) {
        auto fit = value + 1; // even configurations that satisfy no clauses should have SOME fitness
        fitness.push_back(fit);
    }

    newPop = getElites(population, fitness);

    std::vector<unsigned int> wheel(POP_SIZE);
    wheel[0] = fitness[0];
    for (int i = 1; i < POP_SIZE; i++) {
        wheel[i] = wheel[i - 1] + fitness[i];
    }

    auto wheelEnd = wheel[POP_SIZE - 1];

    for (int i = ELITISM_TRESHHOLD; i < POP_SIZE - RANDOMS; i++) {
        auto needle = randomSubunitary() * wheelEnd;

        int which = 0;
        while (needle > wheel[which]) {
            which++;
        }
        newPop.push_back(population[which]);
    }

    for (int i = 0; i < RANDOMS; i++) {
        newPop.push_back(generateBitset(BITS)); // completely random individuals
    }

    return newPop;
}

result geneticSearch(const problem &problem) {
    double multiplier = 1.0;
    unsigned int bEval = 100000000;
    unsigned int oldBEval = 0;
    unsigned int consecutiveStagnation = 0;
    bitset bIndividual;

    BITS = problem.literals_no;

    const auto startTime = Clock::now();

    auto population = generatePopulation();
    int generation = 1;

    while (generation < GEN_LIMIT) {
        crossover(population);
        mutate_SAT(population, problem);
        population = select(population, problem, bEval, bIndividual, multiplier);

        generation++;

        auto crtTime = Clock::now();
        auto crtDuration = std::chrono::duration_cast<std::chrono::milliseconds>(crtTime - startTime).count();
        if (crtDuration > MS_PER_ATTEMPT || bEval == problem.clauses_no) { // timeout or problem solved
            break;
        }

        if (oldBEval > 0 && oldBEval == bEval) {
            consecutiveStagnation++;
        } else {
            consecutiveStagnation = 0;
        }

        if (consecutiveStagnation > 0 && consecutiveStagnation % CONSECUTIVE_STAGNATION_MODULO == 0) {
            multiplier = MULTIPLIER_SPIKE;
        } else {
            multiplier = 1;
        }
        oldBEval = bEval;
    }

    const auto endTime = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return {bEval, bIndividual, duration};
}

result randomGeneration(const problem &problem) {
    const auto startTime = Clock::now();

    unsigned int bEval = 0;
    bitset bIndividual;

    unsigned int cEval, i = 0;
    bitset cIndividual;
    while (true) {
        cIndividual = generateBitset(problem.clauses_no);
        cEval = evalIndividual(problem, cIndividual);
        if (cEval > bEval) {
            bEval = cEval;
            bIndividual = cIndividual;
        }
        i++;
        if (i >= 200) {
            auto crtTime = Clock::now();
            auto crtDuration = std::chrono::duration_cast<std::chrono::milliseconds>(crtTime - startTime).count();
            if (crtDuration > MS_PER_ATTEMPT || bEval == problem.clauses_no) { // timeout or problem solved
                break;
            }
        }
    }

    const auto endTime = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return {bEval, bIndividual, duration};
}
