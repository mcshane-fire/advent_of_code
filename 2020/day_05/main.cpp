#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <set>

std::pair<int,int> find_highest(std::vector<std::string> &passes) {
    int max = 0;
    int mine = -1;
    std::set<int> seats;

    for(auto &s : passes) {
        int full = 0;
        int i=0;
        for(auto c = s.rbegin(); c != s.rend(); c++) {
            if(*c == 'B' || *c == 'R') {
                full |= 1<<i;
            }
            i++;
        }
        max = std::max(max, full);
        seats.insert(full);
    }

    for(auto n : seats) {
        if(seats.contains(n+2) && !seats.contains(n+1)) {
            mine = n+1;
        }
    }

    return {max, mine};
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> passes;

    while(std::getline(input, line)) {
        passes.push_back(line);
    }

    auto [p1,p2] = find_highest(passes);

    std::cout << "Part1: " << p1 << "\n";
    std::cout << "Part2: " << p2 << "\n";

    return 0;
}
