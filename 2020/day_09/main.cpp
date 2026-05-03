#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <algorithm>

std::pair<int64_t,int64_t> find_incorrect(std::vector<int64_t> &numbers, int preamble) {
    for(auto it = numbers.begin() + preamble; it != numbers.end(); it++) {
        std::set<int64_t> valid;
        for(auto n1 = it - preamble; n1 != it; n1++) {
            for(auto n2 = n1 + 1; n2 != it; n2++) {
                if(*n1 != *n2) {
                    valid.insert(*n1 + *n2);
                }
            }
        }
        if(!valid.contains(*it)) {
            for(auto n1 = numbers.begin(); n1 != it-1; n1++) {
                for(auto n2 = n1+2; n2 != it+1; n2++) {
                    if(std::accumulate(n1, n2, 0LL) == *it) {
                        return {*it, *std::max_element(n1, n2) + *std::min_element(n1, n2)};
                    }
                }
            }

            return {*it,0};
        }
    }

    return {0,0};
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    int preamble = argc >= 2 ? 25 : 5;
    std::ifstream input(filename);
    std::string line;

    std::vector<int64_t> numbers;

    while(std::getline(input, line)) {
        numbers.push_back(std::stoll(line));
    }

    auto [p1, p2] = find_incorrect(numbers, preamble);
    std::cout << "Part1: " << p1 << "\n";
    std::cout << "Part2: " << p2 << "\n";

    return 0;
}
