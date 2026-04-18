#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct Pos {
    int x = 0;
    int y = 0;

    bool inmap(std::vector<std::string> &map, int scale) {
        return y >= 0 && y < (scale * map.size()) && x >= 0 && x < (scale * map[0].length());        
    }

    int cost(std::vector<std::string> &map, int scale) {
        int rx = x % map[0].length();
        int ry = y % map.size();
        int mx = x / map[0].length();
        int my = y / map.size();
        int cost = (map[ry][rx] - '0') + mx + my;
        while(cost > 9) {
            cost -= 9;
        }
        return cost;
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

int shortest_path(std::vector<std::string> &map, int scale) {
    std::map<int,std::vector<Pos>> exp = {{0,{{0,0}}}};
    std::map<Pos,int> best = {{{0,0},0}};
    std::vector<Pos> pd = {{1,0},{-1,0},{0,1},{0,-1}};
    Pos end = {(scale*map[0].length())-1,(scale*map.size())-1};

    while(exp.size() > 0) {
        auto e = *(exp.begin());
        exp.erase(e.first);

        for(auto &p : e.second) {
            if(p == end) {
                return e.first;
            }

            for(auto &d : pd) {
                Pos np = p + d;

                if(np.inmap(map,scale) && !best.contains(np)) {
                    int cost = e.first + np.cost(map,scale);
                    best[np] = cost;
                    exp[cost].push_back(np);       
                }
            }
        }
    }

    return 0;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << shortest_path(map, 1) << "\n";
    std::cout << "Part2: " << shortest_path(map, 5) << "\n";

    return 0;
}
