//
// Created by lesa on 27.04.21.
//

#ifndef INC_5_HASH_HASHER_H
#define INC_5_HASH_HASHER_H


#include <string>
#include <utility>
#include <vector>
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
//    const int8_t VECTORS_SIZE = (int8_t) VECTORS.size();
/**
 * bits per block
 */
    const int8_t BITSpBLOCK = 4;

    const std::string fileName{};
    unsigned long filesSize = 0;
public:

    explicit Hasher(char *fileName);

    int hash();

};


#endif //INC_5_HASH_HASHER_H
