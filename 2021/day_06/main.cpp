#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>

int64_t simulate_growth(std::vector<int64_t> ages, int days) {
    for(int i=0; i<days; i++) {
        int64_t next = 0;
        for(int d=8; d>=0; d--) {
            std::swap(next, ages[d]);
        }
        ages[8] = next;
        ages[6] += next;

    }
    return std::accumulate(ages.begin(), ages.end(), 0L);
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int64_t> ages;

    ages.resize(9);

    std::getline(input, line);
    while(true) {
        size_t pos;
        ages[std::stoi(line, &pos)]++;
        if(pos < line.length()) {
            line = line.substr(pos+1);
        } else {
            break;
        }
    }

    std::cout << "Part1: " << simulate_growth(ages, 80) << "\n";
    std::cout << "Part2: " << simulate_growth(ages, 256) << "\n";

    return 0;
}
