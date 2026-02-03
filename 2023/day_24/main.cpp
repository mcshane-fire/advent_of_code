#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <numeric>

struct hailstone {
    int num;
    int64_t p[3];
    int64_t a[3];
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

    if(b.a[0] == 0) {
        return {false, 0.0, 0.0, 0.0};
    }
    double bdyx = (double) b.a[1] / (double) b.a[1];
    double lhs = (double) a.p[1] - b.p[1] - bdyx * (a.p[0] - b.p[0]);
    double rhs = (bdyx * a.a[0]) - (double) a.a[1];
    if(rhs == 0.0) {
        return {false, 0.0, 0.0, 0.0};
    }
    double ta = lhs / rhs;
    return {true, ta, (double) a.p[0] + (a.a[0] * ta), (double) a.p[1] + (a.a[1] * ta)};
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

void analysis(std::vector<struct hailstone>& hailstones) {
    for(int axis=0; axis<3; axis++) {
        std::sort(hailstones.begin(), hailstones.end(), [axis](const hailstone& a, const hailstone& b) {return a.p[axis] < b.p[axis]; });

        std::vector<int64_t> amin(hailstones.size()+1);
        amin[0] = INT64_MAX;
        std::vector<int64_t> amax(hailstones.size()+1);
        amax[hailstones.size()] = INT64_MIN;
        std::transform(hailstones.begin(), hailstones.end(), amin.begin(), std::next(amin.begin()), [axis](const hailstone& h, const int64_t m) { return std::min(m, h.a[axis]); });
        std::transform(hailstones.rbegin(), hailstones.rend(), amax.rbegin(), std::next(amax.rbegin()), [axis](const hailstone& h, const int64_t m) { return std::max(m, h.a[axis]); });

        std::cout << "\n" << axis << "\n";
        std::cout << "     position in [* .. " << (hailstones[0].p[axis]-1) << "] needs velocity in [" << (amax[0]+1) << " .. *]\n";
        for(int i=0; i<hailstones.size(); i++) {
            //std::cout << "  " << i << " num:" << hailstones[i].num << " p:" << hailstones[i].p[axis] << " a:" << hailstones[i].a[axis] << " min:" << amin[i+1] <<  " rmax:" << amax[i] << " " << (amax[i] > amin[i+1] ? "invalid" : "valid") << "\n";

            if(i > 0 && i < hailstones.size()-1) {
                if(amin[i] > amax[i+1]+1) {
                    std::cout << "     position equal to " << hailstones[i].p[axis] << " needs velocity in [" << (amax[i+1]+1) << " .. " << (amin[i]-1) << "]\n";
                }
            } else if(i == 0) {
                std::cout << "     position equal to " << hailstones[i].p[axis] << " needs velocity in [" << (amax[i+1]+1) << " .. *]\n";
            } else {
                std::cout << "     position equal to " << hailstones[i].p[axis] << " needs velocity in [* .. " << (amin[i]-1) << "]\n";
                break;
            }
            if(amin[i+1] > amax[i+1]+1) {
                std::cout << "     position in [" << (hailstones[i].p[axis]+1) << " .. " << (hailstones[i+1].p[axis]-1) << "] (len " << (hailstones[i+1].p[axis] - hailstones[i].p[axis] -1)
                            << ") needs velocity in [" << (amax[i+1]+1) << " .. " << (amin[i+1]-1) << "] (len " << (amin[i+1]-amax[i+1]-1) << ")\n";
            }
        }
        std::cout << "     position in [" << (hailstones.back().p[axis]+1) << " .. *] needs velocity in [* .. " << (amin[hailstones.size()]-1) << "]\n";
    }
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
            struct hailstone h;
            for(int i=0; i<3; i++) {
                h.p[i] = std::stoll(res[i+1]);
                h.a[i] = std::stoll(res[4+i]);
            }
            h.num = hailstones.size();
            hailstones.push_back(h);
        }
    }

    //std::cout << "Part1: " << count_collisions(hailstones, 200000000000000, 400000000000000) << "\n";
    analysis(hailstones);

    return 0;
}
