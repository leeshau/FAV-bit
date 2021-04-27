//
// Created by lesa on 27.04.21.
//

#include <fstream>
#include <limits>
#include <iostream>
#include "Hasher.h"

Hasher::Hasher(char* fileName) : fileName(std::string(fileName)) {
    std::ifstream input{};
    input.open(fileName, std::ios::binary | std::ios::in);
    if (!input.is_open())
        throw std::runtime_error("Invalid filename!");
    input.ignore( std::numeric_limits<std::streamsize>::max() );
    this->filesSize = input.gcount();
    input.close();
}

int Hasher::hash() {
    if (this->filesSize == 0 || this->fileName.empty()) {
        std::cout << "Hasher not initialised properly." << std::endl;
        return 2;
    }



    return 0;
}
