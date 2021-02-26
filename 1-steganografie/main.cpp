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
    const unsigned long latter_size = 310200; //TODO zjisti
//    const unsigned long latter_size = get_size(latter);

//    if (check_sizes(source_size, latter_size) == CHECK_ERROR) {
//        return CHECK_ERROR;
//    }

    int8_t source_byte1, source_byte2, final_byte, bit;
    for (int i = 0; i < latter_size; i += 2) {
        source.seekg(i + MATRIX_BEGIN, std::ios::beg);
        source_byte1 = source.get();
        source_byte2 = source.get();

        final_byte = 0x00;
        //we start from the right side
        for (bit = 0; bit < 4; bit++){
            final_byte = modify_bit(final_byte, bit, ((source_byte2 >> (bit + 4)) & 0b1));
        }
        for (bit = 0; bit < 4; bit++){
            final_byte = modify_bit(final_byte, bit, ((source_byte1 >> (bit + 4)) & 0b1));
        }

        latter.seekp(i / 2, std::ios::beg);
        latter.write((char *)&final_byte, sizeof(int8_t));
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

//    int8_t latter_byte, source_byte, final_byte, offset, bit;
//    for (int i = MATRIX_BEGIN; i < source_size; i++) {
//        latter.seekg(i / 2, std::ios::beg);
//        latter.read((char *)&latter_byte, sizeof(int8_t));
//
//        source.seekg(i, std::ios::beg);
//        source.read((char *)&source_byte, sizeof(int8_t));
//
//        final_byte = source_byte;
//        offset = i % 2; //TODO test
//        for (bit = 0; bit < 4; bit++){
//            final_byte = modify_bit(final_byte, bit * offset, ((latter_byte >> bit) & 0b1));
//        }
//
//        source.seekp(i, std::ios::beg);
//        source.write((char *)&final_byte, sizeof(int8_t));
//    }
    int8_t latter_byte, source_byte, final_byte, offset, bit;
    for (int i = 0; i < latter_size; i++) {
        latter.seekg(i / 2, std::ios::beg);
        latter.read((char *)&latter_byte, sizeof(int8_t));

        source.seekg(i + MATRIX_BEGIN, std::ios::beg);
        source.read((char *)&source_byte, sizeof(int8_t));

        final_byte = source_byte;
        offset = i % 2; //TODO test, maybe (i + 1) % 2
        for (bit = 0; bit < 4; bit++){
            final_byte = modify_bit(final_byte, bit + (4 * offset), ((latter_byte >> bit) & 0b1));
        }

        source.seekp(i + MATRIX_BEGIN, std::ios::beg);
        source.write((char *)&final_byte, sizeof(int8_t));
    }
    source.flush();
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
    int mask = 1 << position;
    return (byte & ~mask) | ((bit << position) & mask);
}

