#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <map>
#include <regex>
#include <numeric>

uint count_steps(std::map<std::string,std::pair<std::string,std::string>>& options, std::string instructions, std::string here = "AAA") {
    uint total = 0;
    auto it = instructions.begin();

    do {
        if(*it == 'L') {
            here = options[here].first;
        } else {
            here = options[here].second;
        }
        total++;
        if(++it == instructions.end()) {
            it = instructions.begin();
        }
    } while(here[2] != 'Z');

    return total;
}

uint64_t count_multiple_paths(std::map<std::string,std::pair<std::string,std::string>>& options, std::string instructions) {
    uint64_t result = 1;

    for(auto p : options) {
        if(p.first[2] == 'A') {
            result = std::lcm(result, count_steps(options, instructions, p.first));
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::string instructions;
    std::regex path = std::regex("^([0-9A-Z]+) = \\(([0-9A-Z]*), ([0-9A-Z]*)\\)$");
    std::smatch res;

    std::getline(input, instructions);
    std::getline(input, line);

    std::map<std::string,std::pair<std::string,std::string>> options;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, path)) {
            //std::cout << res[1] << " " << res[2] << " " << res[3] << "\n";
            options[res[1]] = std::pair<std::string,std::string>(res[2], res[3]);
        }
    }

    std::cout << "Part1: " << count_steps(options, instructions) << "\n";
    std::cout << "Part2: " << count_multiple_paths(options, instructions) << "\n";
    
    return 0;
}