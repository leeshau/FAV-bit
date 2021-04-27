//
// Created by lesa on 27.04.21.
//

#ifndef INC_5_HASH_HASHER_H
#define INC_5_HASH_HASHER_H


#include <string>
#include <utility>

class Hasher {
private:
    const std::string fileName{};
    unsigned long filesSize = 0;
public:
    explicit Hasher(char* fileName);

    int hash();

};


#endif //INC_5_HASH_HASHER_H
