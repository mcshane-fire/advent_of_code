#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

int sum_hashes(std::vector<std::string>& instructions) {
    int total = 0;

    for(auto s : instructions) {
        uint h = 0;
        for(char c : s) {
            h = ((h+c) * 17) & 0xff;
        }
        total += h;
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line, part;

    std::vector<std::string> instructions;

    std::getline(input, line);
    std::stringstream ss(line);

    while(getline(ss, part, ',')) {
        instructions.push_back(part);
    }

    std::cout << "Part1: " << sum_hashes(instructions) << "\n";

    return 0;
}
