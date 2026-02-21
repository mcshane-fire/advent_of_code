#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <regex>

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::regex pair = std::regex("^(\\d+)-(\\d+),(\\d+)-(\\d+)$");
    std::smatch res;
    int count_contain = 0;
    int count_overlap = 0;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pair)) {
            if(std::stoi(res[1]) < std::stoi(res[3])) {
                count_contain += std::stoi(res[2]) >= std::stoi(res[4]);
            } else if(std::stoi(res[1]) == std::stoi(res[3])) {
                count_contain += 1;
            } else {
                count_contain += std::stoi(res[2]) <= std::stoi(res[4]);
            }

            if(std::stoi(res[1]) < std::stoi(res[3])) {
                count_overlap += std::stoi(res[2]) >= std::stoi(res[3]);
            } else {
                count_overlap += std::stoi(res[4]) >= std::stoi(res[1]);
            }
        }
    }

    std::cout << "Part1: " << count_contain << "\n";
    std::cout << "Part2: " << count_overlap << "\n";

    return 0;
}