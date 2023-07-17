#include "eliasFano.h"
#include "rankAndSelect.h"
#include "utils.h"
#include <cmath>
#include <iostream>

eliasFano::eliasFano(std::vector<uint64_t> newBitVector)
        : input(newBitVector) {
    // Computing some basic numbers that we need for the computation
    uint64_t n = input.size();
    universe = input.at(n - 1);
    mostSigBits = std::ceil(std::log2(n));
    remainingBits = std::ceil(std::log2(universe) - std::log2(n));

    // Setting the vector for the upper half of msb to the required size and filling it with 0s
    uint64_t upperHalfLength = (3 * n + 1) / 64 + 1;
    upperHalf.resize(upperHalfLength, 0);

    // Setting the vector for the lower half of msb to the required size and filling it with 0s
    // I was very generous with the space here and there is room for optimizing how many uint64_t are actually needed
    uint64_t elementsPerVector = 64 / remainingBits;
    uint64_t lowerHalfLength = n / elementsPerVector + 1;
    lowerHalf.resize(lowerHalfLength, 0);

    for (int i = 0; i < input.size(); i++) {
        // Fill upper half
        uint64_t MSB = input.at(i) >> remainingBits;
        utils::setBit(upperHalf, i + MSB);

        // Fill lower half
        uint64_t vectorIndex = i / elementsPerVector;
        uint64_t bitmask = ~(((1ULL << mostSigBits) - 1ULL) << remainingBits);
        uint64_t LSB = input.at(i) & bitmask;

        // We shift the LSB into the bitvector from left to right
        lowerHalf[vectorIndex] = lowerHalf[vectorIndex] | LSB << remainingBits * (i % elementsPerVector);
    }

    // Creating the rank and select data structure on the msb for later usage
    upperRankAndSelect = rankAndSelect(upperHalf);
}

uint64_t eliasFano::pred(uint64_t x) {
    // Handeling edge cases
    if(universe < x) {
        return universe;
    }
    if(x < input[0]) {
        return UINT64_MAX;
    }

    // Determining the MSB of the query and the position in the vector for the upper half
    uint64_t MSB = x >> remainingBits;
    uint64_t position = upperRankAndSelect.select(false, MSB);
    uint64_t elementsPerVector = 64 / remainingBits;

    // We iterate over all 1 in this block and save the predecessor, if we find it
    uint64_t result = x + 1;
    for (int i = 0; utils::accessBit(position + 1 + i, upperHalf); i++) {
        // Decode the number with the MSB and the lower bits by using bitmasks
        uint64_t vectorIndex = (position - MSB + 1 + i) / elementsPerVector;
        uint64_t indexInVector = ((position - MSB + 1 + i) * remainingBits) % (elementsPerVector * remainingBits);
        uint64_t bitmask = (((1ULL << remainingBits) - 1ULL) << indexInVector);
        uint64_t lowerBits = (lowerHalf.at(vectorIndex) & bitmask);
        uint64_t completeNumber = (MSB << remainingBits) | (lowerBits >> indexInVector);

        if (completeNumber <= x) {
            result = completeNumber;
        } else {
            break;
        }
    }

    if (result <= x) {
        return result;
    }

    // If we haven't found a predecessor yet, then we scan for the highest bit in the block to the right
    // until we find a block of 1s. There has to be one otherwise x would be less than the smallest input
    while(!utils::accessBit(position, upperHalf)) {
        position--;
        MSB--;
    }

    // Decode the found number and return the result
    uint64_t vectorIndex = (position - MSB) / elementsPerVector;
    uint64_t indexInVector = ((position - MSB) * remainingBits) % (elementsPerVector * remainingBits);
    uint64_t bitmask = (((1ULL << remainingBits) - 1ULL) << indexInVector);
    uint64_t lowerBits = (lowerHalf.at(vectorIndex) & bitmask);
    uint64_t completeNumber = (MSB << remainingBits) | (lowerBits >> indexInVector);

    return completeNumber;
}

/**
 * A helper function to compute the used space
 */
int eliasFano::requiredSpace() {
    return (upperHalf.size() + lowerHalf.size() + 3) * 64 + upperRankAndSelect.requiredSpace();

}



