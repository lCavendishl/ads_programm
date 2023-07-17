#ifndef ADVANCED_DATASTRUCTURES_RANGEMINIMUMQUERIE_H
#define ADVANCED_DATASTRUCTURES_RANGEMINIMUMQUERIE_H

#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>

/**
 * Data structure for the range minimum queries using the O(n log n) datastructure
 */
class rangeMinimumQuerie {
private:
    std::vector<uint64_t> bitVector;
    std::vector<std::vector<uint64_t>> solutionsOfLength2k;

public:
    rangeMinimumQuerie(std::vector<uint64_t> newBitVector);

    std::vector<std::vector<uint64_t>> createNlogNTable(std::vector<uint64_t> block);
    uint64_t rmq(uint64_t start, uint64_t end);
    int requiredSpace();

};

#endif //ADVANCED_DATASTRUCTURES_RANGEMINIMUMQUERIE_H
