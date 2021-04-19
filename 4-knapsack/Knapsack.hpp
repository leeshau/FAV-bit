//
// Created by lesa on 18.04.21.
//
#pragma once

#include <string>
#include <vector>

class Knapsack {

private:
    std::vector<std::size_t> privateKey{};
    std::vector<std::size_t> publicKey{};
    std::string outputFile;
    size_t privateKeySum = 0;

    static bool areCoprime(unsigned long q, unsigned long p);

    static size_t getInvP(size_t p, size_t q);

    void addPrivateKeyElement(unsigned long val);

    std::vector<size_t> getKeyValIx(const size_t &value, size_t invP, size_t q, std::ofstream &off);

public:
    Knapsack(const std::string &privateKeyFileName, const std::string &outputFile);

    static bool isNumber(const std::string &str);

    void makePublicKey(unsigned long p, unsigned long q);

    std::vector<size_t> cypher(std::basic_string<unsigned char> message);

    std::basic_string<unsigned char> decypher(const std::vector<size_t> &cypher, size_t i, size_t i1);


};

