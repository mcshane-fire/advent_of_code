#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <set>

struct Range {
    int64_t r[2];

    bool operator<(const Range &p) const {
        return r[0] == p.r[0] ? r[1] < p.r[1] : r[0] < p.r[0];
    }

    int64_t size() const {
        return r[1] - r[0] + 1;
    }

    void split(const Range &p, std::set<Range> &output) const {
        if(p.r[0] > r[0]) {
            output.insert({r[0], p.r[0]-1});
        }
        if(p.r[1] < r[1]) {
            output.insert({p.r[1]+1, r[1]});
        }
        output.insert({std::max(r[0], p.r[0]), std::min(r[1], p.r[1])});
    }
};

struct Cuboid {
    bool on;
    Range x;
    Range y;
    Range z;

    Cuboid(Range _x, Range _y, Range _z, bool _on = true) : x(_x), y(_y), z(_z), on(_on) {}

    bool in(int px, int py, int pz) {
        return px >= x.r[0] && px <= x.r[1] && py >= y.r[0] && py <= y.r[1] && pz >= z.r[0] && pz <= z.r[1];
    }

    void remove(Cuboid &c, std::vector<Cuboid> &output) {
        // remove c from this, and add the remaining parts into output
        // e.g. if they don't overlap at all, this is added to output
        // e.g. if c entirely covers this, nothing is added to output
        if(c.x.r[0] > x.r[1] || c.x.r[1] < x.r[0] || c.y.r[0] > y.r[1] || c.y.r[1] < y.r[0] || c.z.r[0] > z.r[1] || c.z.r[1] < z.r[0]) {
            output.push_back(*this);
            return;
        }

        std::set<Range> split[3];
        x.split(c.x, split[0]);
        y.split(c.y, split[1]);
        z.split(c.z, split[2]);

        for(auto &sx : split[0]) {
            for(auto &sy : split[1]) {
                for(auto &sz : split[2]) {
                    if(c.x.r[0] > sx.r[1] || c.x.r[1] < sx.r[0] || c.y.r[0] > sy.r[1] || c.y.r[1] < sy.r[0] || c.z.r[0] > sz.r[1] || c.z.r[1] < sz.r[0]) {
                        output.push_back({sx, sy, sz});
                    }
                }
            }
        }
    }
};

int count_on_limit(std::vector<Cuboid> &cuboids, int limit) {
    int total = 0;
    for(int x=-limit; x<=limit; x++) {
        for(int y=-limit; y<=limit; y++) {
            for(int z=-limit; z<=limit; z++) {
                bool state = false;
                for(auto &c : cuboids) {
                    if(c.on != state && c.in(x, y, z)) {
                        state = !state;
                    }
                }
                if(state) {
                    total++;
                }
            }
        }
    }

    return total;
}

int64_t count_on(std::vector<Cuboid> &cuboids) {
    std::vector<Cuboid> on;
    for(auto &cuboid : cuboids) {
        std::vector<Cuboid> left_on;

        for(auto &con : on) {
            con.remove(cuboid, left_on);
        }

        if(cuboid.on) {
            left_on.push_back(cuboid);
        }

        on.clear();
        on.swap(left_on);
    }

    int64_t ret = 0;
    for(auto &c : on) {
        ret += c.x.size() * c.y.size() * c.z.size();
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex pat = std::regex("(on|off) x=([-\\d]+)..([-\\d]+),y=([-\\d]+)..([-\\d]+),z=([-\\d]+)..([-\\d]+)");
    std::smatch res;
    std::vector<Cuboid> cuboids;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            cuboids.push_back({{std::stoi(res[2]), std::stoi(res[3])}, {std::stoi(res[4]), std::stoi(res[5])}, {std::stoi(res[6]), std::stoi(res[7])}, res[1] == "on"});
        }
    }
    
    std::cout << "Part1: " << count_on_limit(cuboids, 50) << "\n";
    std::cout << "Part2: " << count_on(cuboids) << "\n";

    return 0;
}
