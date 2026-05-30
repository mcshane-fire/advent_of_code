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

    Cube(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

    auto operator<=>(const Cube &lhs) const = default;

    Cube operator+(const Cube &c) const {
        return {x+c.x, y+c.y, z+c.z};
    }
};

void print(std::set<Cube> &cubes) {
    int x_min = 0;
    int x_max = 0;
    int y_min = 0;
    int y_max = 0;
    int z_min = 0;
    int z_max = 0;
    for(auto &c : cubes) {
        x_min = std::min(x_min, c.x);
        x_max = std::max(x_max, c.x);
        y_min = std::min(y_min, c.y);
        y_max = std::max(y_max, c.y);
        z_min = std::min(z_min, c.z);
        z_max = std::max(z_max, c.z);
    }

    for(int z=z_min; z<=z_max; z++) {
        std::cout << "z = " << z << "\n";
        for(int y=y_min; y<=y_max; y++) {
            for(int x=x_min; x<=x_max; x++) {
                std::cout << (cubes.contains(Cube(x,y,z)) ? '#' : '.');
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

int count_active(std::set<Cube> cubes, int steps) {
    std::vector<Cube> cdiff = {
        {-1,-1,-1}, {0,-1,-1}, {1,-1,-1}, {-1,0,-1}, {0,0,-1}, {1,0,-1}, {-1,1,-1}, {0,1,-1}, {1,1,-1},
        {-1,-1,0}, {0,-1,0}, {1,-1,0}, {-1,0,0}, {1,0,0}, {-1,1,0}, {0,1,0}, {1,1,0},
        {-1,-1,1}, {0,-1,1}, {1,-1,1}, {-1,0,1}, {0,0,1}, {1,0,1}, {-1,1,1}, {0,1,1}, {1,1,1}};

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

        //print(cubes);
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

    return 0;
}
