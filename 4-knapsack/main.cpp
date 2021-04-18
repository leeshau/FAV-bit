#include <iostream>
#include <filesystem>
#include <fstream>
#include "Knapsack.h"

const std::string &load(char *filename);
bool isText = false;

int main(int argc, char** argv) {
    if (argc != 4){
        std::cerr << "Wrong amount of parameters!\n";
        return 1;
    }
    if (!Knapsack::isNumber(std::string(argv[2]) + argv[3])) {
        std::cerr << "p or q are not digits!\n";
        return 2;
    }
    Knapsack k{"private_key.txt"};
    k.makePublicKey(std::stoul(argv[2]), std::stoul(argv[3]));\
    const auto &message = load(argv[1]);
    auto cyphered = k.cypher(message);
    return 0;
}

const std::string &load(char *filename) {
    std::filesystem::path messagePath(filename);

    if (!std::filesystem::exists(messagePath))
        throw std::runtime_error("Input path does not exist!");

    isText = messagePath.extension() == ".txt";
    std::ifstream stream(messagePath);

    if (!stream.is_open())
        throw std::runtime_error("Error opening fstream on input file!");

    auto fileSize = (std::streamsize) std::filesystem::file_size(messagePath);
    char data[fileSize];
    std::string message;
    stream.read((char*) data, fileSize);

    for (int i = 0; i < fileSize; i++) {
        if (!isTextFile || (data[i] != '\n' && data[i] != 'r'))
            message.push_back(data[i]);
        //TODO maybe bug
    }
    stream.close();
    return message;
}
