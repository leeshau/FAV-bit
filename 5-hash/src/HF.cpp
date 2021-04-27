//
// Created by lesa on 27.04.21.
//

#include "HF.h"

uint16_t HF::getNewC(unsigned short A, unsigned short B, const uint16_t &M) {
    auto newC = A;
    //A xor B
    newC ^= B;
    //newC xor M
    return newC ^= M;
}

uint16_t HF::getNewA(unsigned short B, unsigned short C, const bool &even, unsigned short D) {
    if (!even)
        //F1(B,C)
        f1(B, C);
    else
        //F2(B,C)
        f2(B, C);

    B ^= D;

    return B;
}

void HF::f1(unsigned short &B, unsigned short &C) {
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


void HF::f2(unsigned short &B, unsigned short &C) {
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
