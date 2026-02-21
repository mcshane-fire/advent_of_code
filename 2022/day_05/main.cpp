#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <regex>

std::string stacking(std::string filename, bool reverse) {
    std::ifstream input(filename);
    std::string line;
    std::regex move = std::regex("^move (\\d+) from (\\d+) to (\\d+)$");
    std::smatch res;

    std::getline(input, line);
    auto stacks = std::vector<std::vector<char>>((line.length()+1)/4);
    
    do {
        if(line.length() > 0) {
            if(line[0] == 'm') {
                if(std::regex_match(line, res, move)) {
                    auto move = std::stoi(res[1]);
                    auto &from = stacks[std::stoi(res[2])-1];
                    auto &to = stacks[std::stoi(res[3])-1];
                    if(reverse) {
                        std::move(from.rbegin(), from.rbegin()+move, std::back_inserter(to));
                    } else {
                        std::move(from.end()-move, from.end(), std::back_inserter(to));
                    }
                    from.resize(from.size() - move);
                }
            }
            else {
                for(int i=0; i<stacks.size(); i++) {
                    char c = line[(i*4)+1];
                    if(c >= 'A' and c <= 'Z') {
                        stacks[i].push_back(c);
                    }
                }
            }
        } else {
            for(auto &s : stacks) {
                std::reverse(s.begin(), s.end());
            }
        }

    }
    while(std::getline(input, line));

    std::string tops;
    for(auto& s : stacks) {
        tops.push_back(s.back());
    }

    return tops;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";

    std::cout << "Part1: " << stacking(filename, true) << "\n";
    std::cout << "Part2: " << stacking(filename, false) << "\n";

    return 0;
}