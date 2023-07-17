#ifndef ADVANCED_DATASTRUCTURES_ELIASFANO_H
#define ADVANCED_DATASTRUCTURES_ELIASFANO_H

#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>
#include "rankAndSelect.h"

/**
 * Data structure for the predecessor queries using the elias-fano encoding. The encoding is based on the lecture slides
 */
class eliasFano {
private:
    std::vector<uint64_t> input;
    std::vector<uint64_t> upperHalf;
    std::vector<uint64_t> lowerHalf;
    uint64_t mostSigBits;
    uint64_t remainingBits;
    uint64_t universe;
    rankAndSelect upperRankAndSelect;



public:
    eliasFano(std::vector<uint64_t> newBitVector);

    uint64_t pred(uint64_t x);

    int requiredSpace();
};

#endif //ADVANCED_DATASTRUCTURES_ELIASFANO_H
