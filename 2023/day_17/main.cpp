#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

struct pos {
    int x, y, dlen;
};

void add(std::map<int,std::vector<struct pos>>& explore, int cost, int x, int y, int dlen) {
    if(!explore.contains(cost)) {
        explore[cost] = std::vector<struct pos>();
    }
    explore[cost].emplace_back(x, y, dlen);
    //std::cout << "   - add " << x << "," << y << ":" << (dlen&3) << "(" << (dlen>>2) << ") == " << cost << "\n";
}

int find_minimal_path(std::vector<std::vector<uint>>& map, int min_length, int max_length) {
    int ret = 0;
    std::vector<std::vector<std::map<int,int>>> history(map.size(), std::vector<std::map<int,int>>(map[0].size(), std::map<int,int>()));
    std::map<int,std::vector<struct pos>> explore;
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    add(explore, map[1][0], 0, 1, 3+4);
    add(explore, map[0][1], 1, 0, 0+4);


    while(explore.size() > 0 && ret == 0) {
        auto pl = *explore.begin();
        explore.erase(explore.begin());

        for(auto p : pl.second) {
            if(!history[p.y][p.x].contains(p.dlen)) {
                history[p.y][p.x][p.dlen] = pl.first;
                //std::cout << p.x << "," << p.y << ":" << (p.dlen & 3) << "(" << (p.dlen>>2) << ") == " << pl.first << "\n";

                if(p.y == map.size()-1 && p.x == map[p.y].size()-1 && (p.dlen >> 2) >= min_length) {
                    //std::cout << "Reached the end\n";
                    ret = pl.first;
                    break;
                }

                for(int d=0; d<4; d++) {
                    int nx = p.x + xd[d];
                    int ny = p.y + yd[d];
                    if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].size() && d != ((p.dlen+2) & 3)) {
                        if(d != (p.dlen & 3) && ((p.dlen >> 2) >= min_length)) {
                            add(explore, pl.first + map[ny][nx], nx, ny, d+4);
                        } else if(d == (p.dlen & 3) && ((p.dlen) >> 2) < max_length) {
                            add(explore, pl.first + map[ny][nx], nx, ny, p.dlen+4);
                        }
                    }
                }
            }
        }
    }

    return ret;
}


int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line, part;

    std::vector<std::vector<uint>> map;

    while(std::getline(input, line)) {
        std::vector<uint> vals;
        for(char c : line) {
            vals.push_back(c - '0');
        }
        map.push_back(vals);
    }

    std::cout << "Part1: " << find_minimal_path(map, 1, 3) << "\n";
    std::cout << "Part2: " << find_minimal_path(map, 4, 10) << "\n";
    return 0;
}


