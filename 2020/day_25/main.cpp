#include <string>
#include <fstream>
#include <iostream>

int step(int64_t subject, int64_t value = 1, int steps = 1) {
    for(int i=0; i<steps; i++) {
        value *= subject;
        value %= 20201227;
    }
    return value;
}

int encryption_key(int public_keys[2]) {
    int loop_size=0;
    int value = 1;
    while(true) {
        value = step(7, value);
        loop_size++;

        if(value == public_keys[0]) {
            //std::cout << "0: " << loop_size << " " << step(public_keys[1], 1, loop_size) << "\n";
            return step(public_keys[1], 1, loop_size);
        }
        if(value == public_keys[1]) {
            //std::cout << "1: " << loop_size << " " << step(public_keys[0], 1, loop_size) << "\n";
            return step(public_keys[0], 1, loop_size);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    int public_keys[2];

    for(int i=0; i<2; i++) {
        std::getline(input, line);
        public_keys[i] = std::stoi(line);
    }

    std::cout << "Part1: " << encryption_key(public_keys) << "\n";

    return 0;
}
