#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

enum Direction {
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST,
};

struct Pos {
    int x = 0;
    int y = 0;

    //0:   0     1     2
    //1:      0     1     2
    //2:   0     1     2 
    //3:      0     1     2

    auto operator<=>(const Pos &p) const = default;

    Pos move(Direction d) const {
        Pos ret;
        if(d == EAST) {
            ret = Pos(x+1, y);
        } else if(d == WEST) {
            ret = Pos(x-1, y);
        } else {
            int ver = d == SOUTH_EAST || d == SOUTH_WEST ? 1 : -1;
            int hor = d == SOUTH_EAST || d == NORTH_EAST ? 1 : -1;
            if(((y & 1) == 1 && hor == -1) || ((y & 1) == 0 && hor == 1)) {
                hor = 0;
            }
            ret = Pos(x+hor, y+ver);
        }
        return ret;
    }

    friend std::ostream& operator<<(std::ostream &os, const Pos &p) {
        os << p.x << "," << p.y;
        return os;
    }
};

int count_black(std::vector<std::string> &paths, int days = 0) {
    std::set<Pos> black;
    std::map<std::string, Direction> sd = {
        {"e", EAST}, {"w", WEST},
        {"ne", NORTH_EAST}, {"nw", NORTH_WEST},
        {"se", SOUTH_EAST}, {"sw", SOUTH_WEST}};

    for(auto s : paths) {
        Pos p = Pos(0,0);
        while(s.length() > 0) {
            if(sd.contains(s.substr(0,1))) {
                p = p.move(sd[s.substr(0,1)]);
                s = s.substr(1);
            } else {
                p = p.move(sd[s.substr(0,2)]);
                s = s.substr(2);
            }
        }
        if(black.contains(p)) {
            black.erase(p);
        } else {
            black.insert(p);
        }
    }

    for(int i=0; i<days; i++) {
        std::map<Pos,int> adj;
        for(auto &p : black) {
            for(auto &d : sd) {
                adj[p.move(d.second)]++;
            }
        }
        std::set<Pos> next;
        for(auto &p : adj) {
            if(p.second == 2 || (p.second == 1 && black.contains(p.first))) {
                next.insert(p.first);
            }
        }
        black.clear();
        black.swap(next);
    }

    return black.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> paths;

    int64_t total = 0;
    while(std::getline(input, line)) {
        paths.push_back(line);
    }

    std::cout << "Part1: " << count_black(paths) << "\n";
    std::cout << "Part2: " << count_black(paths, 100) << "\n";

    return 0;
}
