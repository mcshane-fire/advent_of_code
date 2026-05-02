#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <set>

int count_any_yes(std::vector<std::vector<std::string>> &groups) {
    int ret = 0;
    for(auto &g : groups) {
        std::set<char> yes;
        for(auto &p : g) {
            for(auto c : p) {
                yes.insert(c);
            }
        }
        ret += yes.size();
    }
    return ret;
}

int count_all_yes(std::vector<std::vector<std::string>> &groups) {
    int ret = 0;
    for(auto &g : groups) {
        std::map<char,int> yes;
        for(auto &p : g) {
            for(auto c : p) {
                yes[c]++;
            }
        }        
        for(auto &p : yes) {
            if(p.second == g.size()) {
                ret++;
            }
        }
    }
    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::vector<std::string>> groups;

    while(std::getline(input, line)) {
        if(groups.size() == 0 || line.length() == 0) {
            groups.push_back({});
        }
        if(line.length() > 0) {
            groups.back().push_back(line);
        }
    }

    std::cout << "Part1: " << count_any_yes(groups) << "\n";
    std::cout << "Part1: " << count_all_yes(groups) << "\n";

    return 0;
}
