#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <climits>

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

int count_flashes(std::vector<std::string> map, int turns) {
    int count = 0;
    char ten = '9'+1;

    std::vector<Pos> nd = {{1,0}, {1,1}, {1,-1}, {0,1}, {0,-1}, {-1,0}, {-1,1}, {-1,-1}};

    for(int i=0; i<turns; i++) {
        std::vector<Pos> flashes;
        if(turns == INT_MAX) {
            count = 0;
        }

        for(int y=0; y<map.size(); y++) {
            for(int x=0; x<map[y].length(); x++) {
                if(++map[y][x] == ten) {
                    flashes.emplace_back(x, y);
                }
            }
        }

        while(flashes.size() > 0) {
            Pos p = flashes.back();
            flashes.pop_back();
            for(auto &d : nd) {
                Pos np = p + d;
                if(np.inmap(map) && map[np.y][np.x] != ten) {
                    if(++map[np.y][np.x] == ten) {
                        flashes.push_back(np);
                    }
                }
            }
        }

        for(int y=0; y<map.size(); y++) {
            for(int x=0; x<map[y].length(); x++) {
                if(map[y][x] == ten) {
                    count++;
                    map[y][x] = '0';
                }
            }
        }

        if(turns == INT_MAX && count == map.size() * map[0].length()) {
            count = i+1;
            break;
        }
    }

    return count;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_flashes(map, 100) << "\n";
    std::cout << "Part2: " << count_flashes(map, INT_MAX) << "\n";

    return 0;
}
