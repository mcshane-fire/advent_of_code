#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <climits>

int find_minimum(std::map<int,int> &positions, bool scale) {
    int small = INT_MAX;
    for(int i=(*positions.begin()).first; i<=(*positions.end()).first; i++) {
        int count = std::accumulate(positions.begin(), positions.end(), 0, [i, scale](int n, std::pair<int,int> p) {
            int d = std::abs(i-p.first);
            if(scale) {
                d = ((d*d) + d) / 2;
            }
            return n + (d * p.second);
        });
        small = std::min(small, count);
    }
    return small;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::map<int,int> positions;

    std::getline(input, line);
    while(true) {
        size_t pos;
        positions[std::stoi(line, &pos)]++;
        if(pos < line.length()) {
            line = line.substr(pos+1);
        } else {
            break;
        }
    }

    std::cout << "Part1: " << find_minimum(positions, false) << "\n";
    std::cout << "Part2: " << find_minimum(positions, true) << "\n";

    return 0;
}
