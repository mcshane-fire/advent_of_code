#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

int tilt_north(std::vector<std::string>& map) {
    int total = 0;

    for(int i=0; i<map[0].length(); i++) {
        auto wit = map.begin();
        for(auto rit = map.begin(); rit != map.end(); rit++) {
            if((*rit)[i] == '#') {
                wit = rit+1;
            } else if((*rit)[i] == 'O') {
                (*rit)[i] = '.';
                (*wit)[i] = 'O';
                total += map.size() - (wit - map.begin());
                wit++;
            }
        }
    }

    /*
    for(auto line : map) {
        std::cout << line << "\n";
    }*/

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << tilt_north(map) << "\n";

    return 0;
}
