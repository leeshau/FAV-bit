//
// Created by lesa on 27.04.21.
//

#include <fstream>
#include <limits>
#include <iostream>
#include "Hasher.h"

Hasher::Hasher(char *fileName) : fileName(std::string(fileName)) {
    std::ifstream input{};
    input.open(fileName, std::ios::binary | std::ios::in);
    if (!input.is_open())
        throw std::runtime_error("Invalid filename!");
    input.ignore(std::numeric_limits<std::streamsize>::max());
    this->filesSize = input.gcount();
    input.close();
    initABCD();
}

int Hasher::hash() {
    if (this->filesSize == 0 || this->fileName.empty()) {
        std::cout << "Hasher not initialised properly." << std::endl;
        return 2;
    }
    std::ifstream input{};
    input.open(fileName, std::ios::binary | std::ios::in);
    if (!input.is_open())
        throw std::runtime_error("Invalid filename on hash, an odd error!");

    //copy
    auto abcd = ABCD;
    uint16_t buff;
    bool even = false; //start with 1
    while (!input.read((char *) &buff, 2).eof()) {
        iterate(abcd, buff, even);
        even = !even;
        buff = 0;
    }
    if (buff != 0) {
        buff = buff >> 8;
        iterate(abcd, buff, !even);
    }
    print(abcd);
    return 0;
}

void Hasher::initABCD() {
    ABCD.reserve(VECTORS.size());
    for (const auto &i : VECTORS) {
        ABCD.push_back(bitsToUInt16(i));
    }
}

u_int16_t Hasher::bitsToUInt16(const std::vector<bool> &vector) {
    u_int16_t res = 0x00;
    int size = (int) vector.size();
    for (int i = 0; i < size; i++) {
        if (vector.at(size - 1 - i))
            res |= 1 << i;
    }
    return res;
}

std::vector<bool> Hasher::UInt16ToBits(const u_int16_t &uint) {
    std::vector<bool> vector{};
    for (int i = 0; i < 8; i++)
        vector.at(15 - i) = (uint & (1 << i)) != 0;
    return vector;
}

void Hasher::iterate(std::vector<u_int16_t> &abcd, const uint16_t &M, const bool &even) {
    auto &A = abcd.at(0);
    auto &B = abcd.at(1);
    auto &C = abcd.at(2);
    auto &D = abcd.at(3);

    //from A
    auto newC = HF::getNewC(A, B, M);

    //from B
    auto newA = HF::getNewA(B, C, even, D);

    //from C
    auto newD = C;

    //from D
    auto newB = D;

    A = newA;
    B = newB;
    C = newC;
    D = newD;
}

void Hasher::print(const std::vector<u_int16_t> &vector) {
    std::cout << std::hex;
//    std::cout << 10;
    for (auto uint : vector) {
        uint16_t mostLeft = 0x0F & (uint >> 12);
        uint16_t left = 0x0F & (uint >> 8);
        uint16_t right = 0x0F & (uint >> 4);
        uint16_t mostRight(0x0F & uint);

//        std::cout << mostLeft << std::endl;
//        std::cout << left << std::endl;
//        std::cout << right << std::endl;
//        std::cout << mostRight << std::endl;
        std::cout << mostLeft << left << " "
            << right << mostRight << " ";
    }
}


