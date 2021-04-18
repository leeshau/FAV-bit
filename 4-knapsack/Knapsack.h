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

public:
    Knapsack(const std::string &privateKeyFileName, const std::string &outputFile);

    static bool isNumber(const std::string& str);

    void makePublicKey(unsigned long p, unsigned long q);

    void addPrivateKeyElement(unsigned long val);

    static bool areCoprime(unsigned long q, unsigned long p);

    std::vector<size_t> cypher(std::basic_string<unsigned char> message);

    void log(const std::vector<size_t>& vector);

    void log(const std::vector<size_t> &vector, const bool hex);
};

