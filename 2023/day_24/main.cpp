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
    int64_t v[3];
    int64_t time;
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

    if(b.v[0] == 0) {
        return {false, 0.0, 0.0, 0.0};
    }
    double bdyx = (double) b.v[1] / (double) b.v[1];
    double lhs = (double) a.p[1] - b.p[1] - bdyx * (a.p[0] - b.p[0]);
    double rhs = (bdyx * a.v[0]) - (double) a.v[1];
    if(rhs == 0.0) {
        return {false, 0.0, 0.0, 0.0};
    }
    double ta = lhs / rhs;
    return {true, ta, (double) a.p[0] + (a.v[0] * ta), (double) a.p[1] + (a.v[1] * ta)};
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

struct collision_times {
    std::vector<std::pair<int,double>> times;
    int64_t pos;
    int64_t vel;
    int64_t pos_range;
    int64_t vel_range;
};

void generate_order(std::vector<collision_times>& all_times, std::vector<struct hailstone>& hailstones, int64_t pos_l, int64_t pos_u, int64_t vel_l, int64_t vel_u, int axis) {
    int64_t psteps = std::min(20LL, pos_u-pos_l);
    for(int i=0; i<=psteps; i++) {
        int64_t pos = pos_l + (psteps ? (i * (pos_u - pos_l)) / psteps : 0);

        int64_t vsteps = std::min(20LL, vel_u-vel_l);
        for(int j=0; j<=vsteps; j++) {
            int64_t vel = vel_l + (vsteps ? (j * (vel_u - vel_l)) / vsteps : 0);
            //std::cout << "Trying p:" << pos << " v:" << vel << ": ";
            collision_times ct(std::vector<std::pair<int,double>>(hailstones.size()), pos, vel, pos_u-pos_l, vel_u-vel_l);
            std::transform(hailstones.begin(), hailstones.end(), ct.times.begin(),
                [axis,pos,vel](const hailstone& h) { return std::pair<int,double>(h.num, (double) (pos - h.p[axis]) / (double) (h.v[axis] - vel)); });
            std::sort(ct.times.begin(), ct.times.end(), [](const std::pair<int,double> &a, const std::pair<int,double> &b) { return a.first < b.first; });
            all_times.push_back(ct);
        }
    }
}

struct match_result {
    double score;
    int b0;
    int b1;
};

struct match_result find_closest_match(std::vector<collision_times> &a, std::vector<collision_times> &b) {
    int b0 = -1;
    int b1 = -1;
    double best = 0;

    for(int i=0; i < a.size(); i++) {
        for(int j=0; j < b.size(); j++) {
            collision_times &p = a[i];
            collision_times &n = b[j];

            double total = std::inner_product(p.times.begin(), p.times.end(), n.times.begin(), 0.0, std::plus<double>(),
                [](const std::pair<int,double> &a, const std::pair<int,double> &b) {
                    if(a.second!=a.second || b.second!=b.second){return 0.0;}; return std::abs(a.second-b.second);});

            if(b0 == -1 || total < best) {
                b0 = i;
                b1 = j;
                best = total;
            }
        }
    }
    return {best, b0, b1};
}

int64_t analysis(std::vector<struct hailstone>& hailstones) {
    std::vector<std::vector<collision_times>> all_times;

    for(int axis=0; axis<3; axis++) {
        std::vector<collision_times> axis_times;
        std::sort(hailstones.begin(), hailstones.end(), [axis](const hailstone& a, const hailstone& b) {return a.v[axis] < b.v[axis]; });
        int64_t vel_range = std::max(10LL, (hailstones.back().v[axis] - hailstones[0].v[axis]) / 10LL);

        std::sort(hailstones.begin(), hailstones.end(), [axis](const hailstone& a, const hailstone& b) {return a.p[axis] < b.p[axis]; });
        int64_t pos_range = std::max(10LL, (hailstones.back().p[axis] - hailstones[0].p[axis]) / 50LL);

        std::vector<int64_t> amin(hailstones.size()+1);
        amin[0] = INT64_MAX;
        std::vector<int64_t> amax(hailstones.size()+1);
        amax[hailstones.size()] = INT64_MIN;
        std::transform(hailstones.begin(), hailstones.end(), amin.begin(), std::next(amin.begin()), [axis](const hailstone& h, const int64_t m) { return std::min(m, h.v[axis]); });
        std::transform(hailstones.rbegin(), hailstones.rend(), amax.rbegin(), std::next(amax.rbegin()), [axis](const hailstone& h, const int64_t m) { return std::max(m, h.v[axis]); });

        //std::cout << "\n" << axis << "\n";
        //std::cout << "     position in [* .. " << (hailstones[0].p[axis]-1) << "] needs velocity in [" << (amax[0]+1) << " .. *]\n";
        generate_order(axis_times, hailstones, hailstones[0].p[axis] - pos_range, hailstones[0].p[axis]-1, amax[0]+1, amax[0]+vel_range, axis);
        for(int i=0; i<hailstones.size(); i++) {
            //std::cout << "  " << i << " num:" << hailstones[i].num << " p:" << hailstones[i].p[axis] << " a:" << hailstones[i].v[axis] << " min:" << amin[i+1] <<  " rmax:" << amax[i] << " " << (amax[i] > amin[i+1] ? "invalid" : "valid") << "\n";

            if(i > 0 && i < hailstones.size()-1) {
                if(amin[i] > amax[i+1]+1) {
                    //std::cout << "     position equal to " << hailstones[i].p[axis] << " needs velocity in [" << (amax[i+1]+1) << " .. " << (amin[i]-1) << "]\n";
                    generate_order(axis_times, hailstones, hailstones[i].p[axis], hailstones[i].p[axis], amax[i+1]+1, amin[i]-1, axis);
                }
            } else if(i == 0) {
                //std::cout << "     position equal to " << hailstones[i].p[axis] << " needs velocity in [" << (amax[i+1]+1) << " .. *]\n";
                generate_order(axis_times, hailstones, hailstones[i].p[axis], hailstones[i].p[axis], amax[i+1]+1, amax[i+1]+vel_range, axis);
            } else {
                //std::cout << "     position equal to " << hailstones[i].p[axis] << " needs velocity in [* .. " << (amin[i]-1) << "]\n";
                generate_order(axis_times, hailstones, hailstones[i].p[axis], hailstones[i].p[axis], amin[i]-vel_range, amin[i]-1, axis);
                break;
            }
            if(amin[i+1] > amax[i+1]+1) {
                //std::cout << "     position in [" << (hailstones[i].p[axis]+1) << " .. " << (hailstones[i+1].p[axis]-1) << "] (len " << (hailstones[i+1].p[axis] - hailstones[i].p[axis] -1)
                //            << ") needs velocity in [" << (amax[i+1]+1) << " .. " << (amin[i+1]-1) << "] (len " << (amin[i+1]-amax[i+1]-1) << ")\n";

                generate_order(axis_times, hailstones, hailstones[i].p[axis]+1, hailstones[i+1].p[axis]-1, amax[i+1]+1, amin[i+1]-1, axis);
            }
        }
        //std::cout << "     position in [" << (hailstones.back().p[axis]+1) << " .. *] needs velocity in [* .. " << (amin[hailstones.size()]-1) << "]\n";
        generate_order(axis_times, hailstones, hailstones.back().p[axis]+1, hailstones.back().p[axis]+pos_range, amin[hailstones.size()]-vel_range, amin[hailstones.size()]-1, axis);

        all_times.push_back(axis_times);
    }

    auto mr = find_closest_match(all_times[0], all_times[1]);

    collision_times p = all_times[0][mr.b0];
    collision_times n = all_times[1][mr.b1];

    while((p.pos_range > 1 || p.vel_range > 1 || n.pos_range > 1 || n.vel_range > 1) && mr.score > 0.0) {
        std::vector<collision_times> zoom0;
        std::vector<collision_times> zoom1;
        generate_order(zoom0, hailstones, p.pos - (p.pos_range/20), p.pos + (p.pos_range/20), p.vel - (p.vel_range/20), p.vel + (p.vel_range/20), 0);
        generate_order(zoom1, hailstones, n.pos - (n.pos_range/20), n.pos + (n.pos_range/20), n.vel - (n.vel_range/20), n.vel + (n.vel_range/20), 1);
        mr = find_closest_match(zoom0, zoom1);

        collision_times p1 = zoom0[mr.b0];
        collision_times n1 = zoom1[mr.b1];
        p = p1;
        n = n1;
    }
    
    if(mr.score == 0.0) {
        std::vector<collision_times> best;
        best.push_back(p);
        auto mr2 = find_closest_match(best, all_times[2]);
        if(mr2.score == 0.0) {
            return p.pos + n.pos + all_times[2][mr2.b1].pos;
        }
    }

    return 0;
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
                h.v[i] = std::stoll(res[4+i]);
            }
            h.num = hailstones.size();
            h.time = 0;
            hailstones.push_back(h);
        }
    }

    std::cout << "Part1: " << count_collisions(hailstones, 200000000000000, 400000000000000) << "\n";
    std::cout << "Part2: " << analysis(hailstones) << "\n";

    return 0;
}
