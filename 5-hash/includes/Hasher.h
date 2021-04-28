//
// Created by lesa on 27.04.21.
//

#ifndef INC_5_HASH_HASHER_H
#define INC_5_HASH_HASHER_H


#include <string>
#include <utility>
#include <vector>
#include <HF.h>
/**
 * stands for file void, is set as an empty character for file input
 */
#define FVOID '~'

class Hasher {

private:
    const std::vector<std::vector<bool>> VECTORS{
    /*A*/  {0, 0, 1, 0,    1, 1, 1, 0,     1, 1, 0, 0,     0, 0, 0, 0},
    /*B*/  {1, 1, 1, 0,    0, 0, 1, 1,     1, 0, 1, 1,     1, 1, 0, 0},
    /*C*/  {1, 1, 1, 1,    1, 1, 0, 1,     0, 0, 0, 1,     0, 1, 1, 1},
    /*D*/  {1, 0, 1, 0,    0, 0, 0, 1,     0, 0, 1, 1,     1, 1, 1, 1}
    };

    /**
     * bits per block
    */
    const int8_t BITSpBLOCK = 4;

    const std::string fileName;
    unsigned long filesSize = 0;
    std::vector<u_int16_t> ABCD{};
public:

    explicit Hasher(char *fileName);

    int hash();

    void initABCD();

    static u_int16_t bitsToUInt16(const std::vector<bool> &vector);

    static void iterate(std::vector<u_int16_t> &abcd, const uint16_t &M, const bool &even);

    static void print(const std::vector<u_int16_t>& vector);
};


#endif //INC_5_HASH_HASHER_H
