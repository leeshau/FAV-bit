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

    static uint16_t getNewC(unsigned short A, unsigned short B, const uint16_t &M);

    static uint16_t getNewA(unsigned short B, unsigned short C, const bool &even, unsigned short D);

    static void f1(unsigned short &B, unsigned short &C);

    static void f2(unsigned short &B, unsigned short &C);
};


#endif //INC_5_HASH_HF_H
