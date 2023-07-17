#include "rangeMinimumQuerie.h"

rangeMinimumQuerie::rangeMinimumQuerie(std::vector<uint64_t> newBitVector) : bitVector(newBitVector) {
    solutionsOfLength2k = createNlogNTable(bitVector);
}

std::vector<std::vector<uint64_t>> rangeMinimumQuerie::createNlogNTable(std::vector<uint64_t> block) {
    // Matrix with n vectors initialised
    std::vector<std::vector<uint64_t>> matrix(block.size());

    // Initialise with the interval of 2^0=1 aka themselves
    for (uint64_t i = 0; i < block.size(); ++i) {
        matrix[i] = std::vector<uint64_t>{i};
    }

    // Compute intervals of all possible lengths 2^i for each position
    int i = 1;
    while (std::pow(2, i) <= block.size()) {
        for (int j = 0; j < block.size() - std::pow(2, i) + 1; j++) {
            uint64_t pred1 = matrix.at(j).at(i - 1);
            uint64_t pred2 = matrix.at(j + std::pow(2, i - 1)).at(i - 1);
            // Store the index of the lower number from the two previous intervals
            uint64_t newEntry = (bitVector.at(pred1) < bitVector.at(pred2)) ? pred1 : pred2;
            matrix[j].emplace_back(newEntry);
        }
        i++;
    }

    return matrix;
}

uint64_t rangeMinimumQuerie::rmq(uint64_t start, uint64_t end) {
    // Check two intervals that overlap the given interval and compare their lowest index for the solution
    uint64_t l = std::floor(std::log2(end - start + 1));
    uint64_t m1 = solutionsOfLength2k[start][l];
    uint64_t start2 = end - std::pow(2, l) + 1;
    uint64_t m2 = solutionsOfLength2k[start2][l];

    return (bitVector.at(m1) < bitVector.at(m2)) ? m1 : m2;
}

/**
 * A helper function to compute the used space
 */
int rangeMinimumQuerie::requiredSpace() {
    return (solutionsOfLength2k.size() * solutionsOfLength2k[0].size()) * 64;
}