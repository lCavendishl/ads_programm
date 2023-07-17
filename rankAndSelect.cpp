#include "rankAndSelect.h"
#include "utils.h"
#include <cmath>
#include <iostream>

rankAndSelect::rankAndSelect() {}

/**
 * The data structures for rank and select are computed separately
 *
 * @param newBitVector : The bitvector that the queries are based on
 */
rankAndSelect::rankAndSelect(std::vector<uint64_t> newBitVector) : bitVector(newBitVector) {
    creatingRankDatastructure();

    creatingSelectDatastructure();
}

void rankAndSelect::creatingRankDatastructure() {
    // block size and super block size are choosen as instructed in the lecture
    blockSize = std::floor(std::log(bitVector.size() * 64) / 2);
    superBlockSize = std::pow(blockSize, 2);

    // rank0(0) = 0 and rank1(0) = 0
    uint64_t alpha = 0;
    uint64_t beta = 0;
    numberOfSuperBlock.emplace_back(alpha);
    numberOfBlock.emplace_back(beta);

    // We iterate over the entire vector and count the number of 0s and 1s for each (super) block
    for (uint64_t i = 0; i < bitVector.size() * 64; i++) {
        if (!utils::accessBit(i, bitVector)) {
            alpha++;
            beta++;
        }

        if ((i + 1) % superBlockSize == 0) {
            numberOfSuperBlock.emplace_back(alpha);
            beta = 0;
        }
        if ((i + 1) % blockSize == 0) {
            numberOfBlock.emplace_back(beta);
        }
    }

    // Filling the lookuptable of the size 2^s * s. Each solution for all positions is stored
    uint64_t counter;
    std::vector<uint64_t> row;
    for (uint64_t i = 0; i < std::pow(2, blockSize); i++) {
        counter = 0;
        row.clear();
        for (int j = 0; j < blockSize; j++) {
            row.emplace_back(counter);
            if (!((i >> (blockSize - 1 - j)) & 1ULL)) {
                counter++;
            }
        }
        lookuptable.emplace_back(row);
    }
}

void rankAndSelect::creatingSelectDatastructure() {
    // select0(0) = 0 and select1(0) = 0
    solutions0.emplace_back(0);
    solutions1.emplace_back(0);

    // We iterate over the entire vector and count the number of 0s and 1s
    for(uint64_t i = 0; i < bitVector.size() * 64; i++) {
        if(utils::accessBit(i, bitVector)) {
            solutions1.emplace_back(i);
        } else {
            solutions0.emplace_back(i);
        }
    }
}

/**
 * We assume rank0 to be the base case and rank1 has to be computed from that
 */
uint64_t rankAndSelect::rank(bool zeroOrone, int i) {
    // If the index is at the end of a (super) block, then we immediately get an answer
    if (i % superBlockSize == 0) {
        return numberOfSuperBlock.at(i / superBlockSize);
    }
    if (i % blockSize == 0) {
        return numberOfSuperBlock.at(i / superBlockSize) + numberOfBlock.at(i / blockSize);
    }

    // Otherwise the index is in a block and we have to compute the solution
    uint64_t block = bitVector[i / 64];
    uint64_t beginningOfBlock = ((i / blockSize) * blockSize) % 64;

    // The easiest case. The index is in one block and not at the end of the bitvector
    if (beginningOfBlock <= 64 - blockSize) {
        // we use a bitmask to extract a block and lookup the value of the position in this block
        uint64_t bitmask = ((1ULL << blockSize) - 1) << (i % 64);
        uint64_t extractedBits = (block & bitmask) >> (i % 64);

        // The solution is a combination of all 0s from all previos (super) blocks and what we find in the lookup table
        return numberOfSuperBlock.at(i / superBlockSize) + numberOfBlock.at(i / blockSize)
               + lookuptable.at(extractedBits).at(i % blockSize);
    // The block can be at the end of the bit vector
    } else if (((i / blockSize) * blockSize) + blockSize > bitVector.size()) {
        uint64_t bitmask1 = (1ULL << 64 - beginningOfBlock) - 1;
        uint64_t extractedValue = (block & bitmask1);

        return numberOfSuperBlock.at(i / superBlockSize) + numberOfBlock.at(i / blockSize)
               + lookuptable.at(extractedValue).at(i % blockSize);
    // The block can span over two uint64_t and we have to extract the block from both of them
    } else {
        uint64_t block2 = bitVector[(i / 64) + 1];

        uint64_t bitmask1 = (1ULL << 64 - beginningOfBlock) - 1;
        uint64_t bitmask2 = ((1ULL << blockSize - (64 - beginningOfBlock)) - 1) << 64 - (blockSize - (64 - beginningOfBlock));
        uint64_t extractedValue1 = (block & bitmask1) << blockSize - (64 - beginningOfBlock);
        uint64_t extractedValue2 = (block2 & bitmask2) >> 64 - (blockSize - (64 - beginningOfBlock));
        uint64_t mergedValue = extractedValue1 | extractedValue2;

        return numberOfSuperBlock.at(i / superBlockSize) + numberOfBlock.at(i / blockSize)
               + lookuptable.at(mergedValue).at(i % blockSize);
    }
}

/**
 * Select is computed naivly and we just have to read the correct solution from our two vectors
 */
int rankAndSelect::select(bool zeroOrone, int j) {
    if(zeroOrone) {
        return solutions1.at(j);
    } else {
        return solutions0.at(j);
    }
}

/**
 * A helper function to compute the used space
 */
int rankAndSelect::requiredSpace() {
    return (numberOfSuperBlock.size() + numberOfBlock.size() + (lookuptable.size() * lookuptable[0].size()) + solutions0.size() + solutions1.size() + 2) * 64;
}
