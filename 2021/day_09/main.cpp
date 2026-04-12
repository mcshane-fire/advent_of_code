#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

struct Pos {
    int x = 0;
    int y = 0;

    bool inmap(std::vector<std::string> &map) {
        return y >= 0 && y < map.size() && x >= 0 && x < map[y].length();        
    }

    Pos operator+(const Pos &p) const {
        return {x+p.x, y+p.y};
    }

    bool operator==(const Pos &p) const {
        return x == p.x && y == p.y;
    }

    bool operator<(const Pos &p) const {
        return x == p.x ? (y < p.y) : (x < p.x);
    }
};


int find_basin(std::vector<std::string> &map, Pos p) {
    std::set<Pos> basin;
    std::vector<Pos> exp = {p};
    Pos pd[] = {{1,0}, {0,-1}, {-1,0}, {0,1}};

    while(exp.size() > 0) {
        p = exp.back();
        exp.pop_back();

        if(!basin.contains(p)) {
            basin.insert(p);

            for(int d=0; d<4; d++) {
                Pos np = p + pd[d];
                if(np.inmap(map) && map[np.y][np.x] != '9' && map[np.y][np.x] > map[p.y][p.x]) {
                    exp.push_back(np);
                }
            }
        }
    }

    return basin.size();
}

int find_minima(std::vector<std::string> &map, bool basins) {
    int ret = 0;
    Pos pd[] = {{1,0}, {0,-1}, {-1,0}, {0,1}};
    std::vector<int> basin_size;

    for(int y=0; y<map.size(); y++) {
        for(int x=0; x<map[y].length(); x++) {
            int larger = false;
            Pos p = {x,y};
            for(int d=0; d<4; d++) {
                Pos np = p + pd[d];
                if(np.inmap(map) && map[np.y][np.x] <= map[y][x]) {
                    larger = true;
                    break;
                }
            }
            if(!larger) {
                if(basins) {
                    basin_size.push_back(find_basin(map, p));
                } else {
                    ret += map[y][x] - '0' + 1;
                }
            }
        }
    }

    if(basins) {
        std::sort(basin_size.begin(), basin_size.end());
        std::reverse(basin_size.begin(), basin_size.end());
        ret = basin_size[0] * basin_size[1] * basin_size[2];
    }
    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << find_minima(map, false) << "\n";
    std::cout << "Part2: " << find_minima(map, true) << "\n";

    return 0;
}
