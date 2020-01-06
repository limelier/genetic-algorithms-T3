#ifndef T1_PROJECT_UTIL_RANDOM_H_
#define T1_PROJECT_UTIL_RANDOM_H_

bool randomBool();
size_t randomIndex(size_t size);
double randomSubunitary();
void shufflePopulation(std::vector<std::vector<bool>> &population);

#endif //T1_PROJECT_UTIL_RANDOM_H_
