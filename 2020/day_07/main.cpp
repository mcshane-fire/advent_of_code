#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <set>

struct Bag {
    std::string colour;
    std::map<std::string,int> holds;

    Bag() {
    }

    Bag(std::string line) {
        std::regex pat = std::regex("^(.*) bags contain(.*)$");
        std::regex bag = std::regex("(\\d+) ([^.,]*) bag");
        std::smatch res;
        if(std::regex_search(line, res, pat)) {
            colour = res[1];
            line = res[2];
            std::string::const_iterator search(line.begin());
            while(std::regex_search(search, line.cend(), res, bag)) {
                holds[res[2]] = std::stoi(res[1]);
                search = res.suffix().first;
            }
        }
    }
};

int count_holders(std::vector<Bag> &bags, std::string colour) {
    std::set<std::string> valid;
    std::vector<std::string> exp = {colour};

    while(exp.size() > 0) {
        std::string v = exp.back();
        exp.pop_back();
        for(auto &p : bags) {
            if(!valid.contains(p.colour)) {
                if(p.holds.contains(v)) {
                    valid.insert(p.colour);
                    exp.push_back(p.colour);
                }
            }
        }
    }

    return valid.size();
}

int count_contents(std::map<std::string,Bag> &bag, std::string colour) {
    int ret = 1;

    for(auto &p : bag[colour].holds) {
        ret += count_contents(bag, p.first) * p.second;
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<Bag> bags;
    std::map<std::string,Bag> bag;

    while(std::getline(input, line)) {
        bags.push_back(line);
        bag[bags.back().colour] = bags.back();
    }

    std::cout << "Part1: " << count_holders(bags, "shiny gold") << "\n";
    std::cout << "Part2: " << count_contents(bag, "shiny gold")-1 << "\n";

    return 0;
}
