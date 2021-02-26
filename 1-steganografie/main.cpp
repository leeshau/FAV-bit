#include <iostream>
#include <fstream>
#include <vector>

#define OK_RETURN 0
#define ERROR_INPUT_RETURN 1
#define CHECK_ERROR 2
#define CHECK_OK 3
#define UNEXPECTED_RETURN 5
#define MATRIX_BEGIN 54

int merge(std::fstream &source, std::fstream &latter);

int extract(std::fstream &source, std::fstream &latter);

unsigned long get_size(std::fstream &fstream);

int check_sizes(unsigned long source, unsigned long latter);

/**
 * @param byte byte to be modified
 * @param position pos from right in the byte, max 7
 * @param bit to set 0 or 1
 * @return new modified byte
 */
int8_t modify_bit(int8_t byte, int position, bool bit);

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Add mode (MERGE or EXTRACT) and two images as parameters (the former will be the source).\n";
        return ERROR_INPUT_RETURN;
    }

    ///streams
    const std::string mode = std::string(argv[1]);
    std::fstream source, latter;
    std::vector<std::fstream> vec{};
    source.open(argv[2]);
    if (mode == "EXTRACT"){
        latter.open(argv[3], std::fstream::out);
    } else if (mode == "MERGE") {
        latter.open(argv[3]);
    } else {
        std::cerr << "Mode " << mode << " not allowed!\n";
        return ERROR_INPUT_RETURN;
    }

    if (!source.is_open() || !latter.is_open()) {
        std::cerr << "One of the files doesn't exist!\n";
        return ERROR_INPUT_RETURN;
    }

    ///operations
    if (mode == "MERGE") {
        return merge(source, latter);
    } else if (mode == "EXTRACT") {
        return extract(source, latter);
    }

    return UNEXPECTED_RETURN;
}

int extract(std::fstream &source, std::fstream &latter) {
    const unsigned long source_size = get_size(source);
    unsigned long latter_size;
    source.seekg(-sizeof(unsigned long), std::ios::end);
    source.read((char *)&latter_size, sizeof(unsigned long));
    std::cout << "Size of latter is [" << latter_size << "].\n";

    if (check_sizes(source_size, latter_size) == CHECK_ERROR) {
        return CHECK_ERROR;
    }

    int8_t source_byte1, source_byte2, latter_byte, bit;
    for (int i = MATRIX_BEGIN, j = 0; i < source_size && j < latter_size; i += 2, j++) {
        source.seekg(i, std::ios::beg);
        source_byte1 = source.get();
        source_byte2 = source.get();

        latter_byte = 0x00;
        //we start from the right side
        for (bit = 0; bit < 4; bit++){
            latter_byte = modify_bit(latter_byte, bit, ((source_byte2 >> bit) & 0b1));
        }
        for (bit = 4; bit < 8; bit++){
            latter_byte = modify_bit(latter_byte, bit, ((source_byte1 >> (bit - 4)) & 0b1));
        }
        latter.seekp(j, std::ios::beg);
        latter.put(latter_byte);
    }
    latter.flush();
    return OK_RETURN;
}


int merge(std::fstream &source, std::fstream &latter) {
    const unsigned long source_size = get_size(source);
    const unsigned long latter_size = get_size(latter);

    if (check_sizes(source_size, latter_size) == CHECK_ERROR) {
        return CHECK_ERROR;
    }

    int8_t latter_byte, source_byte1, source_byte2, bit;
    for (int i = MATRIX_BEGIN, j = 0; i < source_size && j < latter_size; i += 2, j++) {
        latter.seekg(j, std::ios::beg);
        latter_byte = latter.get();

        source.seekg(i, std::ios::beg);
        source_byte1 = source.get();
        source_byte2 = source.get();

        for (bit = 0; bit < 4; bit++){
            //left side of the latter_byte
            source_byte1 = modify_bit(source_byte1, bit, ((latter_byte >> (bit + 4)) & 0b1));
            //right side of the latter_byte
            source_byte2 = modify_bit(source_byte2, bit, ((latter_byte >> bit) & 0b1));
        }

        source.seekp(i, std::ios::beg);
        source.put(source_byte1);
        source.put(source_byte2);
    }
    source.flush();

    //save the original size for extraction
    source.seekp(-sizeof(unsigned long), std::ios::end);
    source.write((char *)&latter_size, sizeof(unsigned long));

    return OK_RETURN;
}

int check_sizes(unsigned long source, unsigned long latter) {
    if ((latter - MATRIX_BEGIN) > (source - MATRIX_BEGIN) / 2) {
        std::cerr
                << "Latter file is too big!\n"
                << "Max size of latter is " << source / 2000 << " KB.\n";
        return CHECK_ERROR;
    }
    return CHECK_OK;
}

unsigned long get_size(std::fstream &fstream) {
    fstream.seekg(0, std::ios::end);
    const unsigned long size = fstream.tellg();
    fstream.seekg(0, std::ios::beg);
    std::cout << "Size of file [" << size << "].\n";
    return size;
}

int8_t modify_bit(int8_t byte, int position, bool bit) {
    if (position > 7){
        std::cerr << "Cannot move more than 7 bits left!\n";
        return byte;
    }
    int mask = 1 << position;
    return (byte & ~mask) | ((bit << position) & mask);
}
