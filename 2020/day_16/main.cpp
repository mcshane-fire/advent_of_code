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

int count_invalid(std::vector<Constraint> &constraints, std::vector<std::vector<int>> &tickets, std::vector<std::vector<int>> &valid_tickets) {
    int ret = 0;
    for(auto it = std::next(tickets.begin()); it != tickets.end(); it++) {
        bool valid = true;
        for(int n : *it) {
            bool met = false;
            auto cit = constraints.begin();
            while(cit != constraints.end() && !met) {
                met = (*cit).in(n);
                cit++;
            }
            if(!met) {
                ret += n;
                valid = false;
            }
        }
        if(valid) {
            valid_tickets.push_back(*it);
        }
    }

    return ret;
}

int bitcount(int n) {
    int count = 0;
    while(n) {
        n &= n-1;
        count++;
    }
    return count;
}

int64_t assign_fields(std::vector<Constraint> &constraints, std::vector<std::vector<int>> &tickets, std::vector<int> &my_ticket) {
    auto possible = std::vector<int>(constraints.size(), (1<<constraints.size())-1);

    for(auto &t : tickets) {
        for(int i=0; i<t.size(); i++) {
            for(int j=0; j<constraints.size(); j++) {
                if(!constraints[j].in(t[i])) {
                    possible[j] &= ~(1<<i);
                }
            }
        }
    }

    bool progress = true;
    while(progress) {
        progress = false;
        for(int i=0; i<possible.size(); i++) {
            if(bitcount(possible[i]) == 1) {
                for(int j=0; j<possible.size(); j++) {
                    if(i != j && (possible[i] & possible[j]) != 0) {
                        progress = true;
                        possible[j] &= ~possible[i];
                    }
                }
            }
        }
    }

    int64_t ret = 1;
    for(int i=0; i<constraints.size(); i++) {
        if(constraints[i].field.substr(0,9) == "departure") {
            int field = 0;
            while(possible[i] != 1) {
                field++;
                possible[i] >>= 1;
            }
            ret *= my_ticket[field];
        }
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input2.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex cpat = std::regex("([a-z ]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
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

    std::vector<std::vector<int>> valid_tickets;

    std::cout << "Part1: " << count_invalid(constraints, tickets, valid_tickets) << "\n";
    std::cout << "Part2: " << assign_fields(constraints, valid_tickets, tickets[0]) << "\n";

    return 0;
}
