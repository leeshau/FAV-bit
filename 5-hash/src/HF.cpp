//
// Created by lesa on 27.04.21.
//

#include "HF.h"

uint16_t HF::getNewC(uint16_t A, uint16_t B, const uint16_t &M) {
    //A xor B
    A ^= B;
    //newC xor M
    A ^= M;
    return A;
}

uint16_t HF::getNewA(uint16_t B, uint16_t C, const bool &even, uint16_t D) {
    if (!even)
        //F1(B,C)
        f1(B, C);
    else
        //F2(B,C)
        f2(B, C);

    B ^= D;

    return B;
}

void HF::f1(uint16_t &B, uint16_t &C) {
    auto leftB = B;
    auto leftC = C;

    leftB &= leftC;
//TODO strip down redundant C copies after the task is done
    auto rightB = ~B;
    auto rightC = C;

    rightB &= rightC;

    leftB ^= rightB;
    B = leftB;
}


void HF::f2(uint16_t &B, uint16_t &C) {
    auto leftB = B;
    auto leftC = C;

    leftB |= leftC;
//TODO strip down redundant C copies after the task is done
    auto rightB = B;
    auto rightC = C;

    rightB ^= rightC;

    leftB &= rightB;
    B = leftB;
}
