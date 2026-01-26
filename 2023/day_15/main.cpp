#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

uint hash(std::string s) {
    uint h = 0;
    for(char c : s) {
        h = ((h+c) * 17) & 0xff;
    }
    return h;
}

int sum_hashes(std::vector<std::string>& instructions) {
    int total = 0;

    for(auto s : instructions) {
        total += hash(s);
    }

    return total;
}

int follow_instructions(std::vector<std::string>& instructions) {
    int total = 0;
    std::regex rins = std::regex("^([a-z]+)([-=])(\\d*)$");
    std::smatch res;

    std::vector<std::vector<std::pair<std::string,int>>> hashmap(256, std::vector<std::pair<std::string,int>>());

    for(auto ins : instructions) {
        if(std::regex_match(ins, res, rins)) {
            //std::cout << ins << " :" << hash(res[1]) << " " << res[2] << " " << res[3] << "\n";

            uint b = hash(res[1]);
            bool found = false;
            //std::cout << " looking through " << hashmap[b].size() << "\n";
            for(auto it = hashmap[b].begin(); it != hashmap[b].end(); it++) {
                if((*it).first == res[1]) {
                    if(res[2] == "-") {
                        //std::cout << " - erasing element\n";
                        hashmap[b].erase(it);
                    } else {
                        //std::cout << " - replacing value\n";
                        (*it).second = std::stoi(res[3]);
                    }
                    found = true;
                    break;
                }
            }
            if(res[2] == "=" && !found) {
                //std::cout << " - added to back of list\n";
                hashmap[b].emplace_back(res[1], std::stoi(res[3]));
            }
        }
    }

    for(int i=0; i<hashmap.size(); i++) {
        if(hashmap[i].size() > 0) {
            //std::cout << i << ": ";
            for(int j=0; j<hashmap[i].size(); j++) {
                //std::cout << "[" << hashmap[i][j].first << " " << hashmap[i][j].second << "]";
                total += (i+1) * (j+1) * hashmap[i][j].second;
            }
            //std::cout << "\n";
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line, part;

    std::vector<std::string> instructions;

    std::getline(input, line);
    std::stringstream ss(line);

    while(getline(ss, part, ',')) {
        instructions.push_back(part);
    }

    std::cout << "Part1: " << sum_hashes(instructions) << "\n";
    std::cout << "Part2: " << follow_instructions(instructions) << "\n";

    return 0;
}
