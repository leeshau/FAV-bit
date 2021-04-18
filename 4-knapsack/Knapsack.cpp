//
// Created by lesa on 18.04.21.
//

#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Knapsack.h"
#include <bitset>
#include <numeric>

Knapsack::Knapsack(const std::string &privateKeyFileName, const std::string &outputFile) {
    this->outputFile = outputFile;
    const std::ifstream pkStream(privateKeyFileName);
    if (!pkStream)
        throw std::runtime_error("Error while opening private key file!");

    std::string line{};
    std::size_t pos;
    while ((pos = line.find(',')) != std::string::npos) {
        std::string keyValue = line.substr(0, pos);
        if (!isNumber(keyValue))
            throw std::runtime_error("Token [" + keyValue + "] is not a number in private key!");

        addPrivateKeyElement(std::stoul(keyValue));
        line.erase(0, pos + 1);
    }

    if (!line.empty()) {
        if (!isNumber(line))
            throw std::runtime_error("Token [" + line + "] is not a number in private key's last line!");
        addPrivateKeyElement(std::stoul(line));
    }
    if (privateKey.empty())
        throw std::runtime_error("Private key is empty!");
    if (publicKey.size() % 8 != 0)
        throw std::runtime_error("Size of the public key must be divisible by 8!");
}

bool Knapsack::isNumber(const std::string& str) {
    for (auto c : str){
        if (!isdigit(c))
            return false;
    }
    return true;
}

void Knapsack::makePublicKey(const unsigned long p, const unsigned long q) {
    if (q <= privateKeySum)
        throw std::runtime_error("Parameter q has to be bigger than sum of private key!");
    if (!areCoprime(q, p))
        throw std::runtime_error("Parameters q and p have to be co-prime!");

    publicKey.reserve(privateKey.size());
    for (const size_t &keyValue : privateKey)
        publicKey.push_back(keyValue * p % q);
}

void Knapsack::addPrivateKeyElement(unsigned long val) {
    if (privateKeySum >= val)
        throw std::runtime_error("Private key must be a super-increasing sequence!");
    privateKeySum += val;
    privateKey.push_back(val);
}

bool Knapsack::areCoprime(const unsigned long q, const unsigned long p) {
    return std::gcd(q, p) == 1;
}

std::vector<std::size_t> Knapsack::cypher(std::basic_string<unsigned char> message) {
    u_int i, j, k;
    for (i = 0; i < (message.size() * 8) % publicKey.size(); i += 8)
        message.push_back(0);

    std::vector<std::size_t> cyphered;
    for (i = 0; i < message.size(); i += publicKey.size() / 8) {
        std::size_t txtVal = 0;
        for (j = 0; j < publicKey.size() / 8; j++) {
            for (k = 0; k < 8; k++) {
                if ((message[i + j] >> k) & 0b1) {
                    txtVal += publicKey[j * 8 + k];
                }
            }
        }
        cyphered.push_back(txtVal);
    }
    log(cyphered, true);
    return cyphered;
}

void Knapsack::log(const std::vector<size_t>& vector, const bool hex) {
    std::ofstream off(this->outputFile, std::ios::out | std::ios::binary);
    if (!off.is_open())
        throw std::runtime_error("Error while opening the output stream!");
    if (hex)
        off << std::hex;
    else
        off << std::dec;

    for (const auto &c : vector)
        off << c << ", ";
    off << std::endl;
}
