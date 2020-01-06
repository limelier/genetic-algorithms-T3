#include <cmath>
#include <functional>
#include "../util/random.h"

#include "bitset.h"
std::vector<bool> generateBitset(size_t size) {
    std::vector<bool> bitset;
    for (size_t i = 0; i < size; i++) {
        bitset.push_back(bool(randomBool()));
    }
    return bitset;
}
std::vector<std::vector<bool>> generateNeighbors(std::vector<bool> &bitset) {
    const size_t size = bitset.size();
    std::vector<std::vector<bool>> neighbors;
    typeof(bitset) neighbor;
    for (size_t i = 0; i < size; i++) {
        neighbor = bitset;
        neighbor[i] = !neighbor[i];
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

std::vector<bool> mergeBitsets(const std::vector<std::vector<bool>>& bitsets) {
    std::vector<bool> result;
    const size_t chunks = bitsets.size();
    const size_t chunkSize = bitsets[0].size();
    const size_t size = chunks * chunkSize;

    result.reserve(size);
    for (auto bitset : bitsets) {
        result.insert(result.end(), bitset.begin(), bitset.end());
    }

    return result;
}
std::vector<std::vector<bool>> splitBitset(std::vector<bool> bitset, size_t chunkSize) {
    std::vector<std::vector<bool>> result;
    const size_t chunks = bitset.size() / chunkSize;

    for (size_t i = 0; i < chunks; i++) {
        const size_t chunkStart = i * chunkSize;
        const size_t chunkEnd = (i + 1) * chunkSize;

        result.emplace_back(bitset.begin() + chunkStart, bitset.begin() + chunkEnd);
    }

    return result;
}

std::pair<std::vector<bool>, std::vector<bool>> cross(std::vector<bool> first, std::vector<bool> second) {
    auto bitmask = generateBitset(first.size());
    for (size_t i = 0; i < bitmask.size(); i++) {
        bool flip = bitmask[i];
        if (flip) {
            std::swap(first[i], second[i]);
        }
    }
    return {first, second};
}

std::vector<bool> greyToBinary(const std::vector<bool> &grey) {
    std::vector<bool> binary(grey.size());
    binary[0] = grey[0];
    for (size_t i = 1; i < grey.size(); i++) {
        if (grey[i]) {
            binary[i] = !binary[i-1];
        } else {
            binary[i] = binary[i-1];
        }
    }

    return binary;
}


