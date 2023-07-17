#ifndef ADVANCED_DATASTRUCTURES_UTILS_H
#define ADVANCED_DATASTRUCTURES_UTILS_H


#include <vector>
#include <iostream>

/**
 * This class handles some utility functions that are used throughout the project
 */
class utils {
public:
    inline static void printBinary(uint64_t number) {
        for (int i = 63; i >= 0; --i) {
            uint64_t bit = (number >> i) & 1;
            std::cout << bit;
        }
    }

    inline static bool accessBit(int i, std::vector<uint64_t> bitVector) {
        uint64_t block = bitVector[i / 64];
        // bits are read right to left
        bool bit = (block >> (i%64)) & 1ULL;
        return bit;
    }

    inline static void setBit(std::vector<uint64_t> &bitVector, int i) {
        int index = i / 64;
        int bitPosition = i % 64;
        bitVector[index] |= (1ULL << bitPosition);
    }

};


#endif //ADVANCED_DATASTRUCTURES_UTILS_H
