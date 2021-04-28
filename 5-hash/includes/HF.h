//
// Created by lesa on 27.04.21.
//

#ifndef INC_5_HASH_HF_H
#define INC_5_HASH_HF_H

#include <cstdint>

/**
 * stands for Hash Functions
 */
class HF {

public:

    static uint16_t getNewC(uint16_t A, uint16_t B, const uint16_t &M);

    static uint16_t getNewA(uint16_t B, uint16_t C, const bool &even, uint16_t D);

    static void f1(uint16_t &B, uint16_t &C);

    static void f2(uint16_t &B, uint16_t &C);
};


#endif //INC_5_HASH_HF_H
