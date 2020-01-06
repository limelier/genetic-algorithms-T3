#ifndef CPP_PROJ_CONFIG_H
#define CPP_PROJ_CONFIG_H

const int ALGORITHM_RUNS = 30;

const int POP_SIZE = 100;
const int GEN_LIMIT = 3000;
const double BASE_MUTATION_CHANCE = 0.2;
const double POSITIVE_MUTATION_CHANCE = 0.5;
const double RANDOM_MUTATION_CHANCE = 0.2;
const int CROSSOVERS = 10;
const int ELITISM_TRESHHOLD = 10;
const int RANDOMS = 10;
const double BASE_MULT = 0.1;
const double CONSECUTIVE_STAGNATION_LIMIT = 5;
const unsigned int CONSECUTIVE_STAGNATION_MODULO = 20;
const double MULTIPLIER_INCREMENT = 0.5;
const double MULTIPLIER_SPIKE = 1;

const int MS_PER_ATTEMPT = 600000; // 10 minutes
#endif //CPP_PROJ_CONFIG_H
