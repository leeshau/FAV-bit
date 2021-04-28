#include <iostream>
#include <Hasher.h>

int main(int argc, char** argv) {
//    std::cout << "Welcome to Hash!" << std::endl;
    if (argc != 2) {
        std::cout << "Insert one parameter with file you want to hash." << std::endl;
        return 1;
    }
    Hasher h(argv[1]);
    return h.hash();
}
