#ifndef ADVANCED_DATASTRUCTURES_RANKANDSELECT_H
#define ADVANCED_DATASTRUCTURES_RANKANDSELECT_H

#include <vector>
#include <cstdint>

/**
 * Data structure for rank and select queries. The rank queries are implemented after the optimal solution and the
 * select queries after the naive solution
 */
class rankAndSelect {
private:
    std::vector<uint64_t> bitVector;

    // Rank
    std::vector<uint64_t> numberOfBlock;
    std::vector<uint64_t> numberOfSuperBlock;
    std::vector<std::vector<uint64_t>> lookuptable;
    uint64_t blockSize;
    uint64_t superBlockSize;
    void creatingRankDatastructure();

    // Select
    std::vector<uint64_t> solutions0;
    std::vector<uint64_t> solutions1;
    void creatingSelectDatastructure();


public:
    rankAndSelect(std::vector<uint64_t> newBitVector);
    rankAndSelect();

    uint64_t rank(bool zeroOrone, int i);
    int select(bool zeroOrone, int j);

    int requiredSpace();
};

#endif //ADVANCED_DATASTRUCTURES_RANKANDSELECT_H
