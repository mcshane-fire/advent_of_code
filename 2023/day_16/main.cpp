#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

struct pos {
    int x, y, d;
};

int count_energised(std::vector<std::string>& map) {
    int total = 0;

    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::vector<std::vector<uint>> history(map.size(), std::vector<uint>(map[0].length(), 0));
    std::vector<struct pos> explore = {{0, 0, 0}};

    while(explore.size() > 0) {
        auto p = explore.back();
        explore.pop_back();

        if(p.y >= 0 && p.y < map.size() && p.x >= 0 && p.x < map[p.y].length() && (history[p.y][p.x] & (1<<p.d)) == 0) {
            //std::cout << "Exp: " << p.y << " " << p.x << " " << p.d << "\n";
            history[p.y][p.x] |= 1<<p.d;
            char c = map[p.y][p.x];

            if(c == '.' || (c == '|' && (p.d & 1)) || (c == '-' && !(p.d & 1))) {
                explore.emplace_back(p.x + xd[p.d], p.y + yd[p.d], p.d);
            } else if(c == '|') {
                explore.emplace_back(p.x, p.y, 1);
                explore.emplace_back(p.x, p.y, 3);
            } else if(c == '-') {
                explore.emplace_back(p.x, p.y, 0);
                explore.emplace_back(p.x, p.y, 2);
            } else if(c == '/') {
                // 0->1  1->0  2->3  3->2
                explore.emplace_back(p.x + xd[p.d^1], p.y + yd[p.d^1], p.d^1);
            } else if(c == '\\') {
                // 0->3  1->2  2->1  3->0
                explore.emplace_back(p.x + xd[3-p.d], p.y + yd[3-p.d], 3-p.d);
            }
        }
    }

    for(auto v : history) {
        total += std::count_if(v.begin(), v.end(), [](int i) { return i != 0; });
    }

    return total;
}


int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line, part;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_energised(map) << "\n";

    return 0;
}


