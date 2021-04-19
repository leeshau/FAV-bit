//
// Created by lesa on 18.04.21.
//

#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Knapsack.hpp"
#include <numeric>

/**
 * generates new instance of Knapsack with new private key and public key
 * the instance is ready to work after this
 * @param privateKeyFileName name of the file that contains the private key
 * @param outputFile where to put the results
 */
Knapsack::Knapsack(const std::string &privateKeyFileName, const std::string &outputFile) {
    this->outputFile = outputFile;
    std::ifstream pkStream(privateKeyFileName);
    if (!pkStream)
        throw std::runtime_error("Error while opening private key file!");

    std::string line{};
    std::getline(pkStream, line);
    std::size_t pos = 0;
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

/**
 * @param str string to check if it is a number
 * @return true if it is
 */
bool Knapsack::isNumber(const std::string &str) {
    for (auto c : str) {
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

/**
 * cyphers the given message
 * @param message text to be cyphered
 * @return cyphered text
 */
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
    std::ofstream off(this->outputFile, std::ios::out | std::ios::binary);
    if (!off.is_open())
        throw std::runtime_error("Error while opening the output stream!");
    off << std::hex;
    for (const auto &c : cyphered)
        off << c << " ";
    off << std::endl;
    off.flush();
    off.close();
    return cyphered;
}

/**
 * decyphers given cyphered message
 * @param cypher cyphered message
 * @param p p parameter for knapsack algorithm
 * @param q q parameter for knapsack algorithm
 * @return string with decyphered message
 */
std::basic_string<unsigned char> Knapsack::decypher(const std::vector<size_t> &cypher, size_t p, size_t q) {
    size_t pInv = getInvP(p, q);

    std::ofstream off(this->outputFile, std::ios::out | std::ios::binary | std::ios::app);
    if (!off.is_open())
        throw std::runtime_error("Error while opening the output stream!");

    std::basic_string<unsigned char> decypher{};
    int i;
    for (const auto &messageValue : cypher) {
        auto keyValIx = getKeyValIx(messageValue, pInv, q, off);
        size_t charIx = 0;
        unsigned char decypheredChar = 0;
        for (i = (int) keyValIx.size() - 1; i >= 0; i--) {
            if ((keyValIx[i] / 8) > charIx)
                do {
                    decypher.push_back(decypheredChar);
                    decypheredChar = 0;
                    charIx++;
                } while ((keyValIx[i] / 8) > charIx);

            decypheredChar |= 1UL << (keyValIx[i] % 8);
        }
        decypher.push_back(decypheredChar);

        if (keyValIx.empty())
            for (i = 0; i < (privateKey.size() / 8) - 1; i++)
                decypher.push_back(0);
        else if ((keyValIx.front() / 8) < ((privateKey.size() / 8) - 1))
            decypher.push_back(0);
    }
    off << std::endl;
    off.flush();
    off << std::dec;
    for (const auto &c: decypher)
        off << c;
    off.flush();
    off.close();
    return decypher;
}

/**
 * gets inverted p according to the Knapsack algorithm
 * @param p p parameter for knapsack algorithm
 * @param q q parameter for knapsack algorithm
 * @return inverted p
 */
std::size_t Knapsack::getInvP(const std::size_t p, const std::size_t q) {
    std::size_t inverseP = 1;
    while ((p * inverseP % q) != 1)
        inverseP++;
    return inverseP;
}

std::vector<size_t> Knapsack::getKeyValIx(const size_t &value, size_t invP, size_t q, std::ofstream &off) {
    size_t decypher = value * invP % q;
    off << std::hex;
    off << decypher << " ";
    std::vector<size_t> keyValIx;
    for (int i = (int) privateKey.size() - 1; i >= 0; i--) {
        if (privateKey[i] <= decypher) {
            decypher -= privateKey[i];
            keyValIx.push_back(i);
        }
        if (decypher == 0)
            break;
    }
    return keyValIx;
}
