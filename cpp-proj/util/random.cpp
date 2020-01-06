#include <cstddef>
#include <random>
#include <functional>
#include "random.h"

auto generator() {
    static std::random_device randomDevice;
    static thread_local auto generator = std::mt19937_64(randomDevice());
    return generator;
}

bool randomBool() {
    static auto gen = std::bind(std::uniform_int_distribution<>(0,1), generator());

    return gen();
}

size_t randomIndex(size_t size) {
    auto gen = std::bind(std::uniform_int_distribution<>(0, size - 1), generator());
    return gen();
}

double randomSubunitary() {
    static auto gen = std::bind(std::uniform_real_distribution<double>(0, 1), generator());
    return gen();
}

void shufflePopulation(std::vector<std::vector<bool>> &population) {
    std::shuffle(population.begin(), population.end(), generator());
}