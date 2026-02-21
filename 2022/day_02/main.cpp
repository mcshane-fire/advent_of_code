#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<int> cap;
    int total = 0;
    int total2 = 0;
    std::map<std::string,int> scores = {{"A X", 4}, {"A Y", 8}, {"A Z", 3},
                                        {"B X", 1}, {"B Y", 5}, {"B Z", 9},
                                        {"C X", 7}, {"C Y", 2}, {"C Z", 6}};

    std::map<std::string,int> score2 = {{"A X", 3}, {"A Y", 4}, {"A Z", 8},
                                        {"B X", 1}, {"B Y", 5}, {"B Z", 9},
                                        {"C X", 2}, {"C Y", 6}, {"C Z", 7}};

    while(std::getline(input, line)) {
        total += scores[line];
        total2 += score2[line];
    }

    std::cout << "Part1: " << total << "\n";
    std::cout << "Part2: " << total2 << "\n";
    
    return 0;
}