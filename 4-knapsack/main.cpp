#include <iostream>
#include <fstream>
#include "Knapsack.hpp"

#if __has_include(<filesystem>)
#include <filesystem>
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
    namespace filesystem = std::experimental::filesystem;
#endif


std::string load(char *filename);

void exportBinary(const std::basic_string<unsigned char> &decyphered, const char *originalFilename);

bool isText = false;

int main(int argc, char** argv) {
    std::cout << "Welcome to Knapsack algorithm.\n";
    if (argc != 4){
        std::cerr << "Wrong amount of parameters!\n";
        return 1;
    }
    if (!Knapsack::isNumber(std::string(argv[2]) + argv[3])) {
        std::cerr << "p or q are not digits!\n";
        return 2;
    }
    Knapsack k{"private_key.txt", "output.txt"};
    std::size_t p = std::stoul(argv[2]), q = std::stoul(argv[3]);
    k.makePublicKey(p, q);
    auto message = load(argv[1]);
    auto cyphered = k.cypher((const std::basic_string<unsigned char> &) message);
    std::cout << "Cyphering done.\n";
    auto decyphered = k.decypher(cyphered, p, q);
    std::cout << "Decyphering done.\n";
    if (!isText)
        exportBinary(decyphered, argv[1]);
    std::cout << "Program successfully finished!\n";
    return 0;
}

/**
 * exports binary file
 * @param decyphered decyphered message
 * @param originalFilename filename to append knapsack_ to it
 */
void exportBinary(const std::basic_string<unsigned char> &decyphered, const char *originalFilename) {
    const std::string filename("knapsack_" + std::string(originalFilename));
    std::ofstream stream(filename, std::ios::out | std::ios::binary);
    stream.write((const char *) decyphered.c_str(), (std::streamsize) decyphered.size());
    stream.flush();
    stream.close();
    std::cout << "Binary file [" << filename << "] has been exported.\n";
}

/**
 * loads file into memory
 * @param filename name of the file
 * @return string of the content in the file
 */
std::string load(char *filename) {
    filesystem::path messagePath(filename);

    if (!filesystem::exists(messagePath))
        throw std::runtime_error("Input path does not exist!");

    isText = messagePath.extension() == ".txt";
    std::ifstream stream(messagePath);

    if (!stream.is_open())
        throw std::runtime_error("Error opening fstream on input file!");

    auto fileSize = (std::streamsize) filesystem::file_size(messagePath);
    char data[fileSize];
    std::string message;
    stream.read((char*) data, fileSize);

    for (int i = 0; i < fileSize; i++) {
        if (!isText || (data[i] != '\n' && data[i] != 'r'))
            message.push_back(data[i]);
        //TODO maybe bug
    }
    stream.close();
    return message;
}
