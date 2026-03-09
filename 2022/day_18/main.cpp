#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>

class block {
    public:
    int x;
    int y;
    int z;

    bool next_to(const block &b) const {
        int xd = std::abs(b.x - x);
        int yd = std::abs(b.y - y);
        int zd = std::abs(b.z - z);

        return ((xd ? 0 : 1) + (yd ? 0 : 1) + (zd ? 0 : 1) == 2 && xd + yd + zd == 1);
    }

    block operator+(const block &b) {
        return {b.x+x, b.y+y, b.z+z};
    }

    bool operator==(const block &b) const {
        return x == b.x && y == b.y && z == b.z; 
    }

    bool operator<(const block &b) const {
        return b.x == x ? (b.y == y ? z < b.z : y < b.y) : x < b.x;
    }
};

int count_surface_area(std::set<block> &blocks) {
    int total = 6 * blocks.size();

    for(auto it = blocks.begin(); it != blocks.end(); it++) {
        for(auto jt = std::next(it); jt != blocks.end(); jt++) {
            if((*it).next_to(*jt)) {
                total-=2;
            }
        }
    }

    return total;
}

int count_exterior_surface_area(std::set<block> &blocks) {
    block b = *(blocks.begin());
    int x_min = std::accumulate(blocks.begin(), blocks.end(), b.x, [](int m, const block &b) { return std::min(b.x, m); }) - 1;
    int x_max = std::accumulate(blocks.begin(), blocks.end(), b.x, [](int m, const block &b) { return std::max(b.x, m); }) + 1;
    int y_min = std::accumulate(blocks.begin(), blocks.end(), b.y, [](int m, const block &b) { return std::min(b.y, m); }) - 1;
    int y_max = std::accumulate(blocks.begin(), blocks.end(), b.y, [](int m, const block &b) { return std::max(b.y, m); }) + 1;
    int z_min = std::accumulate(blocks.begin(), blocks.end(), b.z, [](int m, const block &b) { return std::min(b.z, m); }) - 1;
    int z_max = std::accumulate(blocks.begin(), blocks.end(), b.z, [](int m, const block &b) { return std::max(b.z, m); }) + 1;

    std::set<block> visited = {{x_min, y_min, z_min}};
    std::vector<block> exp = {{x_min, y_min, z_min}};
    std::vector<block> move;

    for(int i=-1; i<=1; i+=2) {
        move.emplace_back(i, 0, 0);
        move.emplace_back(0, i, 0);
        move.emplace_back(0, 0, i);
    }

    while(exp.size() > 0) {
        b = exp.back();
        exp.pop_back();

        for(auto &m : move) {
            block nb = b + m;
            if(nb.x >= x_min && nb.x <= x_max && nb.y >= y_min && nb.y <= y_max && nb.z >= z_min && nb.z <= z_max) {
                if(!visited.contains(nb) && !blocks.contains(nb)) {
                    visited.insert(nb);
                    exp.push_back(nb);
                }
            }
        }  
    }

    for(int x=x_min; x<=x_max; x++) {
        for(int y=y_min; y<=y_max; y++) {
            for(int z=z_min; z<=z_max; z++) {
                block b = {x, y, z};
                if(!visited.contains(b)) {
                    blocks.insert(b);
                }
            }
        }
    }

    return count_surface_area(blocks);
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::set<block> blocks;

    while(std::getline(input, line)) {
        size_t pos1, pos2;
        int x = std::stoi(line, &pos1);
        int y = std::stoi(line.substr(pos1+1), &pos2);
        int z = std::stoi(line.substr(pos1+pos2+2));
        blocks.insert({x, y, z});
    }

    std::cout << "Part1: " << count_surface_area(blocks) << "\n";
    std::cout << "Part2: " << count_exterior_surface_area(blocks) << "\n";

    return 0;
}
