#include <string>
#include <fstream>
#include <iostream>

int64_t compute(std::string line, int &read) {
    int64_t total = 0;
    bool mul = false;
    read = 0;

    while(true) {
        int64_t next;
        if(line[0] == '(') {
            int skip;
            next = compute(line.substr(1), skip);
            line = line.substr(1+skip);
            read += 1+skip;
        } else {
            size_t pos;
            next = std::stoll(line, &pos);
            line = line.substr(pos);
            read += pos;
        }
        if(mul) {
            total *= next;
        } else {
            total += next;
        }

        if(line.length() == 0) {
            break;
        }
        if(line[0] == ')') {
            read++;
            break;
        }
        if(line[0] != ' ' || line.length() < 4 || !(line[1] == '+' || line[1] == '*') || line[2] != ' ') {
            std::cout << "Unexpected item\n";
            break;
        }

        mul = line[1] == '*';
        read += 3;
        line = line.substr(3);
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    int64_t total = 0;
    while(std::getline(input, line)) {
        int read;
        total += compute(line, read);
    }

    std::cout << "Part1: " << total << "\n";

    return 0;
}
