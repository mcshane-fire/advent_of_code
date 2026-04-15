#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

int count_paths(std::map<std::string,std::vector<std::string>> &map, bool single_small_twice) {

    std::vector<std::vector<std::string>> paths;
    std::vector<std::pair<std::vector<std::string>,bool>> exp = {{{"start"},false}};

    while(exp.size() > 0) {
        auto p = exp.back();
        exp.pop_back();

        for(auto &n : map[p.first.back()]) {
            if(n == "end") {
                paths.push_back(p.first);
                paths.back().push_back("end");
            } else {
                int dup = n[0] >= 'A' && n[0] <= 'Z' ? 0 : (std::find(p.first.begin(), p.first.end(), n) == p.first.end() ? 0 : 1);
                if(dup == 0 || (single_small_twice && p.second == false)) {
                    exp.emplace_back(p.first, p.second || dup > 0);
                    exp.back().first.push_back(n);
                }
            }
        }
    }

    return paths.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input1.txt";
    std::ifstream input(filename);
    std::string line;
    std::map<std::string,std::vector<std::string>> map;

    while(std::getline(input, line)) {
        auto it = std::find(line.begin(), line.end(), '-');
        std::string n1 = line.substr(0, std::distance(line.begin(), it));
        std::string n2 = line.substr(std::distance(line.begin(), std::next(it)));
        if(!map.contains(n1)) {
            map[n1] = std::vector<std::string>();
        }
        if(!map.contains(n2)) {
            map[n2] = std::vector<std::string>();
        }
        if(n2 != "start") {
            map[n1].push_back(n2);
        }
        if(n1 != "start") {
            map[n2].push_back(n1);
        }
    }

    std::cout << "Part1: " << count_paths(map, false) << "\n";
    std::cout << "Part2: " << count_paths(map, true) << "\n";

    return 0;
}
