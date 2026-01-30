#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>

struct hailstone {
    int64_t x;
    int64_t y;
    int64_t z;
    int64_t dx;
    int64_t dy;
    int64_t dz;
};

struct collision {
    bool valid;
    double time;
    double x;
    double y;
};

struct collision find_collision(struct hailstone &a, struct hailstone &b) {
    // solve for ta where
    // a.x + (a.dx * ta) == b.x + (b.dx * tb)
    // a.y + (a.dy * ta) == b.y + (b.dy * tb)

    // (a.x - b.x) + (a.dx * ta) = (b.dx * tb)
    // tb = ((a.x - b.x) + (a.dx * ta)) / b.dx
    // 
    // a.y + (a.dy * ta) = b.y + (b.dy / b.dx) * ((a.x - b.x) + (a.dx * ta))
    // a.y - b.y - (b.dy / b.dx) * (a.x - b.x) = ta * (((b.dy / b.dx) * a.dx) - a.dy)

    if(b.dx == 0) {
        return {false, 0.0, 0.0, 0.0};
    }
    double bdyx = (double) b.dy / (double) b.dx;
    double lhs = (double) a.y - b.y - bdyx * (a.x - b.x);
    double rhs = (bdyx * a.dx) - (double) a.dy;
    if(rhs == 0.0) {
        return {false, 0.0, 0.0, 0.0};
    }
    double ta = lhs / rhs;
    return {true, ta, (double) a.x + (a.dx * ta), (double) a.y + (a.dy * ta)};
}

int count_collisions(std::vector<struct hailstone>& hailstones, double min, double max) {
    int total = 0;
    for(auto it = hailstones.begin(); it != hailstones.end(); it++) {
        for(auto nit = it+1; nit != hailstones.end(); nit++) {

            struct hailstone& a = *it;
            struct hailstone& b = *nit;
            //std::cout << "comp " << a.x << " " << b.x << "\n";

            struct collision cola = find_collision(*it, *nit);
            //std::cout << "A: Valid: " << cola.valid << " time:" << cola.time << " x:" << cola.x << " y:" << cola.y << "\n";

            struct collision colb = find_collision(*nit, *it);
            //std::cout << "B: Valid: " << colb.valid << " time:" << colb.time << " x:" << colb.x << " y:" << colb.y << "\n";

            if(cola.valid && colb.valid && cola.time > 0 && colb.time > 0 && 
                cola.x >= min && cola.y >= min && cola.x <= max && cola.y <= max) {
                    //std::cout << " - match\n";
                    total++;
            }
        } 
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::regex pat = std::regex("^(\\d+), (\\d+), (\\d+) @[ ]*([-]*\\d+),[ ]*([-]*\\d+),[ ]*([-]*\\d+)$");
    std::smatch res;
    std::vector<struct hailstone> hailstones;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            //std::cout << res[1] << " " << res[2] << " " << res[3] << " | " << res[4] << " " << res[5] << " " << res[6] << "\n";
            hailstones.emplace_back(std::stoll(res[1]), std::stoll(res[2]), std::stoll(res[3]),
                                    std::stoll(res[4]), std::stoll(res[5]), std::stoll(res[6]));
        }
    }

    std::cout << "Part1: " << count_collisions(hailstones, 200000000000000, 400000000000000) << "\n";

    return 0;
}