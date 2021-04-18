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
    log(cyphered, false);
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
    off.flush();
    off.close();
}

std::string Knapsack::decypher(const std::vector<size_t> &cypher, size_t p, size_t q) {
    size_t pInv = getInvP(p, q);

    std::ofstream off(this->outputFile, std::ios::out | std::ios::binary);
    if (!off.is_open())
        throw std::runtime_error("Error while opening the output stream!");
    off << std::hex;

    std::vector<std::basic_string<unsigned char>> decypher{};
    for (const auto &messageValue : cypher) {
        std::vector<size_t> keyValIx = getKeyValIx(messageValue, pInv, q, off);
        unsigned char decipheredChar = 0;
        size_t charIndex = 0;  /// Index of character in deciphered block - with 16 key length there are 2 chars in the block
        for (size_t i = keyValIx.size(); i > 0; --i) {
            if ((keyValIx[i - 1] / 8) > charIndex) {
                do {    /// If ciphering with 16 / 24 key length and char in the middle is 0, we need to decipher it too
                    decypher.push_back(decipheredChar);
                    decipheredChar = 0;
                    charIndex++;
                } while ((keyValIx[i - 1] / 8) > charIndex);
            }

            decipheredChar |= 1UL << (keyValIx[i - 1] % 8);
        }

        decypher.push_back(decipheredChar);
        if (!keyValIx.empty() && ((keyValIx.front() / 8) < ((privateKey.size() / 8) - 1))) {
            decypher.push_back(0);       /// If ciphering with 16 / 24 key length and last char is 0, we need
        }                                           /// to decipher it too

        if (keyValIx.empty()) {  /// All characters were 0 - we need to decipher them all
            for (int i = 0; i < (privateKey.size() / 8) - 1; ++i) {
                decypher.push_back(0);
            }
        }
    }

    off << std::endl;
    off.flush();
    off.close();
    return decypher;
}

std::size_t Knapsack::getInvP(const std::size_t p, const std::size_t q) {
    std::size_t inverseP = 1;
    while ((p * inverseP % q) != 1) {
        inverseP++;
    }
    return inverseP;
}

std::vector<size_t> Knapsack::getKeyValIx(const size_t &value, size_t invP, size_t q, std::ofstream &off) {
    size_t decypher = value * invP % q;
    off << std::to_string(decypher) << ", ";
    std::vector<size_t> keyValIx;
    for (size_t i = privateKey.size(); i > 0; i--) {
        if (privateKey[i - 1] <= decypher) {
            decypher -= privateKey[i - 1];
            keyValIx.push_back(i - 1);   /// We need to find the indexes of every key value we found
        }
        if (decypher == 0)
            break;
    }
    return keyValIx;
}
