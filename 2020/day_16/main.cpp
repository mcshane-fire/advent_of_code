#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

struct Constraint {
    std::string field;
    int a_low;
    int a_high;
    int b_low;
    int b_high;

    Constraint(std::string _field, int _al, int _ah, int _bl, int _bh) :
        field(_field), a_low(_al), a_high(_ah), b_low(_bl), b_high(_bh) {}

    bool in(int val) {
        return (val >= a_low && val <= a_high) || (val >= b_low && val <= b_high);
    }
};

int count_invalid(std::vector<Constraint> &constraints, std::vector<std::vector<int>> &tickets) {
    int ret = 0;
    for(auto it = std::next(tickets.begin()); it != tickets.end(); it++) {
        for(int n : *it) {
            bool met = false;
            auto cit = constraints.begin();
            while(cit != constraints.end() && !met) {
                met = (*cit).in(n);
                cit++;
            }
            if(!met) {
                ret += n;
            }
        }
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex cpat = std::regex("([a-z]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
    std::smatch res;
    std::vector<Constraint> constraints;
    std::vector<std::vector<int>> tickets;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, cpat)) {
            constraints.emplace_back(res[1], std::stoi(res[2]), std::stoi(res[3]), std::stoi(res[4]), std::stoi(res[5]));
        } else if(line[0] >= '0' && line[0] <= '9') {
            tickets.push_back(std::vector<int>());
            while(line.length() > 0) {
                size_t pos;
                tickets.back().push_back(std::stoi(line, &pos));
                line = line.substr(std::min(line.length(), pos+1));
            }
        }
    }

    std::cout << "Part1: " << count_invalid(constraints, tickets) << "\n";

    return 0;
}
