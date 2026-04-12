#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

struct Problem {
    std::vector<std::vector<std::set<char>>> digits;
    std::vector<std::set<char>> display;

    Problem(std::string line) {
        digits.resize(8);
        for(int i=0; i<10; i++) {
            auto g = std::find(line.begin(), line.end(), ' ');
            std::set<char> ns;
            ns.insert(line.begin(), g);
            digits[ns.size()].emplace_back(ns);
            line = line.substr(std::distance(line.begin(), g)+1);
        }
        line = line.substr(2);
        for(int i=0; i<4; i++) {
            auto g = std::find(line.begin(), line.end(), ' ');
            std::set<char> ns;
            ns.insert(line.begin(), g);
            display.emplace_back(ns);
            if(i<3) {
                line = line.substr(std::distance(line.begin(), g)+1);
            }
        }
    }
};

int count_1478(std::vector<Problem> &problems) {
    int ret = 0;
    std::set<int> id = {2, 3, 4, 7};
    for(auto &p : problems) {
        ret += std::count_if(p.display.begin(), p.display.end(), [id](const std::set<char> &s) { return id.contains(s.size()); });
    }
    return ret;
}

int sum_displays(std::vector<Problem> &problems) {
    int ret = 0;
    std::map<std::set<char>,int> numbers = {{{'a','b','c','e','f','g'}, 0},
                                            {{'c', 'f'}, 1},
                                            {{'a','c','d','e','g'}, 2},
                                            {{'a','c','d','f','g'}, 3},
                                            {{'b','c','d','f'}, 4},
                                            {{'a','b','d','f','g'}, 5},
                                            {{'a','b','d','e','f','g'}, 6},
                                            {{'a','c','f'}, 7},
                                            {{'a','b','c','d','e','f','g'}, 8},
                                            {{'a','b','c','d','f','g'}, 9}};

    for(auto &p : problems) {
        std::map<char,char> mapping;
        std::vector<char> sd;
        std::set_difference(p.digits[3][0].begin(), p.digits[3][0].end(), p.digits[2][0].begin(), p.digits[2][0].end(), std::inserter(sd, sd.begin()));
        mapping[sd[0]] = 'a';

        sd.resize(0);
        for(auto &s : p.digits[6]) {
            std::set_difference(p.digits[2][0].begin(), p.digits[2][0].end(), s.begin(), s.end(), std::inserter(sd, sd.begin()));
            if(sd.size() > 0) {
                mapping[sd[0]] = 'c';
                break;
            }
        }

        mapping[*p.digits[2][0].begin() == sd[0] ? *(std::next(p.digits[2][0].begin())) : *p.digits[2][0].begin()] = 'f';

        sd.resize(0);
        std::set_intersection(p.digits[4][0].begin(), p.digits[4][0].end(), p.digits[5][0].begin(), p.digits[5][0].end(), std::inserter(sd, sd.begin()));
        std::vector<char> sd2;
        std::set_intersection(p.digits[5][1].begin(), p.digits[5][1].end(), p.digits[5][2].begin(), p.digits[5][2].end(), std::inserter(sd2, sd2.begin()));
        std::vector<char> sd3;
        std::set_intersection(sd.begin(), sd.end(), sd2.begin(), sd2.end(), std::inserter(sd3, sd3.begin()));
        mapping[sd3[0]] = 'd';

        sd.resize(0);
        std::set_difference(p.digits[4][0].begin(), p.digits[4][0].end(), p.digits[2][0].begin(), p.digits[2][0].end(), std::inserter(sd, sd.begin()));
        mapping[sd[0] == sd3[0] ? sd[1] : sd[0]] = 'b';

        for(auto &s : p.digits[6]) {
            sd.resize(0);
            std::set_difference(p.digits[7][0].begin(), p.digits[7][0].end(), s.begin(), s.end(), std::inserter(sd, sd.begin()));
            if(!mapping.contains(sd[0])) {
                mapping[sd[0]] = 'e';
                break;
            }
        }

        for(auto c : p.digits[7][0]) {
            if(!mapping.contains(c)) {
                mapping[c] = 'g';
                break;
            }
        }        

        int number = 0;
        for(auto &d : p.display) {
            std::set<char> r;
            for(auto c : d) {
                r.insert(mapping[c]);
            }
            number = (number*10) + numbers[r];
        }

        ret += number;
    }
    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Problem> problems;

    while(std::getline(input, line)) {
        problems.emplace_back(Problem(line));
    }

    std::cout << "Part1: " << count_1478(problems) << "\n";
    std::cout << "Part2: " << sum_displays(problems) << "\n";

    return 0;
}
