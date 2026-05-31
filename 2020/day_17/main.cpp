#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

struct Cube {
    int x;
    int y;
    int z;
    int w;

    Cube(int _x, int _y, int _z, int _w = 0) : x(_x), y(_y), z(_z), w(_w) {}

    auto operator<=>(const Cube &lhs) const = default;

    Cube operator+(const Cube &c) const {
        return {x+c.x, y+c.y, z+c.z, w+c.w};
    }
};

int count_active(std::set<Cube> cubes, int steps, int dimen = 3) {
    std::vector<Cube> cdiff;

    for(int x=-1; x<=1; x++) {
        for(int y=-1; y<=1; y++) {
            for(int z=-1; z<=1; z++) {
                int w_min = dimen == 4 ? -1 : 0;
                int w_max = dimen == 4 ? 1 : 0;
                for(int w=w_min; w<=w_max; w++) {
                    if(!(x==0 && y==0 && z==0 && w==0)) {
                        cdiff.emplace_back(x, y, z, w);
                    }
                }
            }
        }
    }

    for(int i=0; i<steps; i++) {
        std::map<Cube,int> adj;
        for(auto &c : cubes) {
            for(auto &ci : cdiff) {
                adj[c + ci]++;
            }
        }

        std::vector<Cube> to_remove;
        for(auto c : cubes) {
            if(adj[c] < 2 || adj[c] > 3) {
                to_remove.push_back(c);
            }
        }
        for(auto &c : to_remove) {
            cubes.erase(c);
        }

        for(auto p : adj) {
            if(p.second == 3) {
                Cube nc = p.first;
                cubes.insert(p.first);
            }
        }
    }
    return cubes.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::set<Cube> cubes;

    int y = 0;
    while(std::getline(input, line)) {
        for(int x=0; x<line.length(); x++) {
            if(line[x] == '#') {
                cubes.insert(Cube(x, y, 0));
            }
        }
        y++;
    }

    std::cout << "Part1: " << count_active(cubes, 6) << "\n";
    std::cout << "Part2: " << count_active(cubes, 6, 4) << "\n";

    return 0;
}
