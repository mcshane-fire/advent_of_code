#include <string>
#include <fstream>
#include <iostream>
#include <format>

int64_t compute(std::string line, int &read, bool add_first = false) {
    int64_t total = 0;
    bool mul = false;
    read = 0;
    std::string partial = "";

    while(true) {
        int64_t next;
        if(line[0] == '(') {
            int skip;
            next = compute(line.substr(1), skip, add_first);
            line = line.substr(1+skip);
            read += 1+skip;
        } else {
            size_t pos;
            next = std::stoll(line, &pos);
            line = line.substr(pos);
            read += pos;
        }
        if(mul) {
            if(add_first) {
                partial += std::format("{:d} * ", total);
                total = next;
            } else {
                total *= next;
            }
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

    if(add_first) {
        int r;
        partial += std::format("{:d}", total);
        total = compute(partial, r);
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    int64_t part1_total = 0;
    int64_t part2_total = 0;
    while(std::getline(input, line)) {
        int read;
        part1_total += compute(line, read);
        part2_total += compute(line, read, true);
    }

    std::cout << "Part1: " << part1_total << "\n";
    std::cout << "Part2: " << part2_total << "\n";

    return 0;
}
