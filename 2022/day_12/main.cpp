#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <cstdint>

struct pos {
    int x;
    int y;
    int cost = 0;

    bool operator==(const struct pos &a) const {
        return (x == a.x) && (y == a.y);
    }
    bool operator<(const struct pos &a) const {
        return (x < a.x) || (x == a.x && y < a.y);
    }
};

int distance_from_start(std::vector<std::string> &map, pos start, pos end) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::vector<pos> frontier;
    std::set<pos> seen;

    frontier.push_back(start);

    int i=0;
    while(i < frontier.size()) {
        auto& p = frontier[i++];
        if(!seen.contains(p)) {
            seen.insert(p);
            if(p == end) {
                return p.cost;
            }
            for(int dir=0; dir<4; dir++) {
                int ny = p.y+yd[dir];
                int nx = p.x+xd[dir];
                if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length()) {
                    if((map[ny][nx] <= map[p.y][p.x]+1) && !seen.contains({nx, ny})) {
                        frontier.push_back({nx, ny, p.cost+1});
                    }
                }
            }
        }
    }

    return 0;
}

int count_steps(std::vector<std::string> map, bool from_s) {
    pos start;
    pos end;
    std::vector<pos> alist;

    for(int y=0; y<map.size(); y++) {
        int x;
        if((x = map[y].find('S')) != std::string::npos) {
            start = {x, y};
            map[y][x] = 'a';
        }
        if((x = map[y].find('E')) != std::string::npos) {
            end = {x, y};
            map[y][x] = 'z';
        }
        int off = 0;
        while((x = map[y].substr(off).find('a')) != std::string::npos) {
            alist.push_back({x+off, y});
            off += x+1;
        }
    }

    if(from_s) {
        return distance_from_start(map, start, end);
    } else {
        int min = INT32_MAX;
        for(auto &p : alist) {
            int steps = distance_from_start(map, p, end);
            if(steps > 0) {
                min = std::min(min, steps);
            }
        }
        return min;
    }
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_steps(map, true) << "\n";
    std::cout << "Part2: " << count_steps(map, false) << "\n";

    return 0;
}