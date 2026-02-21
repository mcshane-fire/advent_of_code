#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <set>

int score_duals(std::vector<std::string>& packs) {
    std::vector<char> dual;
    for(auto p : packs) {
        auto mid = p.begin() + (p.length()/2);
        std::set<char> p1, p2;
        p1.insert(p.begin(), mid);
        p2.insert(mid, p.end());
        std::set_intersection(p1.begin(), p1.end(), p2.begin(), p2.end(), std::back_inserter(dual));
    }

    return std::accumulate(dual.begin(), dual.end(), 0,
                        [](int acc, char v){return acc + (v >= 'a' ? (v-'a'+1) : (v-'A'+27)); });
}

int score_groups(std::vector<std::string>& packs) {
    std::vector<char> groups;
    for(int i=0; i<packs.size(); i+=3) {
        std::set<char> p1, p2, p3, p4;
        std::vector<char> comb;
        p1.insert(packs[i].begin(), packs[i].end());
        p2.insert(packs[i+1].begin(), packs[i+1].end());
        p3.insert(packs[i+2].begin(), packs[i+2].end());
        std::set_intersection(p1.begin(), p1.end(), p2.begin(), p2.end(), std::back_inserter(comb));
        p4.insert(comb.begin(), comb.end());
        std::set_intersection(p4.begin(), p4.end(), p3.begin(), p3.end(), std::back_inserter(groups));
    }

    return std::accumulate(groups.begin(), groups.end(), 0,
                        [](int acc, char v){return acc + (v >= 'a' ? (v-'a'+1) : (v-'A'+27)); });
}


int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> packs;

    while(std::getline(input, line)) {
        packs.push_back(line);
    }

    std::cout << "Part1: " << score_duals(packs) << "\n";
    std::cout << "Part2: " << score_groups(packs) << "\n";
    
    return 0;
}