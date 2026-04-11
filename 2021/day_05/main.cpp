#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>

int overlapping(std::string filename, bool diagonal) {
    std::ifstream input(filename);
    std::string line;
    std::vector<int> numbers;
    std::vector<std::vector<int>> vents;
    std::regex points = std::regex("^(\\d+),(\\d+) -> (\\d+),(\\d+)$");
    std::smatch res;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, points)) {
            int x1 = std::stoi(res[1]);
            int y1 = std::stoi(res[2]);
            int x2 = std::stoi(res[3]);
            int y2 = std::stoi(res[4]);
            int xd = x2 == x1 ? 0 : (x2 > x1 ? 1 : -1);
            int yd = y2 == y1 ? 0 : (y2 > y1 ? 1 : -1);

            if(!diagonal && x1 != x2 && y1 != y2) {
                continue;
            }

            if(vents.size() <= std::max(y1,y2)) {
                vents.resize(std::max(y1,y2)+1);
            }

            int y=y1;
            int x=x1;
            while(true) {
                if(vents[y].size() <= std::max(x1,x2)) {
                    vents[y].resize(std::max(x1,x2)+1);
                }

                if(y1 == y2) {
                    for(x=std::min(x1,x2); x<=std::max(x1,x2); x++) {
                        vents[y][x]++;
                    }
                } else if(x1 == x2) {
                    vents[y][x1]++;
                } else {
                    vents[y][x]++;
                }

                if(y == y2) {
                    break;
                }
                y+=yd;
                x+=xd;
            }
        }
    }

    int ret = 0;
    for(auto &v : vents) {
        for(auto &n : v) {
            if(n > 1) {
                ret++;
            }
        }
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";

    std::cout << "Part1: " << overlapping(filename, false) << "\n";
    std::cout << "Part2: " << overlapping(filename, true) << "\n";

    return 0;
}
