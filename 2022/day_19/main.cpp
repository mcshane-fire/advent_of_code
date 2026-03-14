#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <regex>
#include <format>

struct blueprint {
    int num;
    int ore_robot_ore;
    int clay_robot_ore;
    int obsidian_robot_ore;
    int obsidian_robot_clay;
    int geode_robot_ore;
    int geode_robot_obsidian;
};

class state {
    public:
    int time_left = 24;
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geode = 0;

    int ore_robot = 1;
    int clay_robot = 0;
    int obsidian_robot = 0;
    int geode_robot = 0;

    int new_ore_robot = 0;
    int new_clay_robot = 0;
    int new_obsidian_robot = 0;
    int new_geode_robot = 0;

    bool block_ore = false;
    bool block_clay = false;
    bool block_obsidian = false;
    bool block_geode = false;

    //std::vector<std::string> history;

    void step(std::string h) {
        time_left--;

        ore += ore_robot;
        clay += clay_robot;
        obsidian += obsidian_robot;
        geode += geode_robot;

        ore_robot += new_ore_robot;
        clay_robot += new_clay_robot;
        obsidian_robot += new_obsidian_robot;
        geode_robot += new_geode_robot;

        if(new_ore_robot + new_obsidian_robot + new_geode_robot > 0) {
            block_clay = false;
        }
        if(new_clay_robot + new_obsidian_robot + new_geode_robot > 0) {
            block_ore = false;
        }
        if(new_ore_robot + new_clay_robot + new_geode_robot > 0) {
            block_obsidian = false;
        }
        if(new_ore_robot + new_clay_robot + new_obsidian_robot > 0) {
            block_geode = false;
        }

        new_ore_robot = 0;
        new_clay_robot = 0;
        new_obsidian_robot = 0;
        new_geode_robot = 0;
      
        //history.push_back(std::format("{} {} {} {} {}", h, ore, clay, obsidian, geode));
    }
};

void make_ore_robot2(blueprint &b, state &s, std::vector<state> &exp) {
    if(s.ore >= b.ore_robot_ore) {
        if(!s.block_ore) {
            state n(s);
            n.ore -= b.ore_robot_ore;
            n.new_ore_robot++;
            n.step("+OR");
            exp.push_back(n);
        }
        s.block_ore = true;
    }
}

void make_clay_robot2(blueprint &b, state &s, std::vector<state> &exp) {
    if(s.ore >= b.clay_robot_ore) {
        if(!s.block_clay) {
            state n(s);
            n.ore -= b.clay_robot_ore;
            n.new_clay_robot++;
            n.step("+CR");
            exp.push_back(n);
        }
        s.block_clay = true;
    }
}

void make_obsidian_robot2(blueprint &b, state &s, std::vector<state> &exp) {
    if(s.ore >= b.obsidian_robot_ore && s.clay >= b.obsidian_robot_clay) {
        if(!s.block_obsidian) {
            state n(s);
            n.ore -= b.obsidian_robot_ore;
            n.clay -= b.obsidian_robot_clay;
            n.new_obsidian_robot++;
            n.step("+OBR");
            exp.push_back(n);
        }
        s.block_obsidian = true;
    }
}

void make_geode_robot2(blueprint &b, state &s, std::vector<state> &exp) {
    if(s.ore >= b.geode_robot_ore && s.obsidian >= b.geode_robot_obsidian) {
        s.ore -= b.geode_robot_ore;
        s.obsidian -= b.geode_robot_obsidian;
        s.new_geode_robot++;
        s.step("+GR");
        exp.push_back(s);
    } else {
        s.step("+++");
        exp.push_back(s);
    }
}

int find_best(blueprint &b, int max_ore) {
    
    std::vector<state> exp = {{}};

    int best = 0;
    int finished = 0;

    //exp[0].history.push_back("Start");

    while(exp.size() > 0) {
        state s = exp.back();
        exp.pop_back();

        if(s.time_left == 0) {
            if(s.geode >= 12) {
                best = std::max(best, s.geode);
            } else if(s.geode > 0) {
                best = std::max(best, s.geode);
            }
            finished++;
            //if(((finished) % 10000) == 0) {
            //    std::cout << finished << " " << exp.size() << " " << best << "\n";
            //}
        } else {    
            std::string step = "---";
            if(s.ore_robot < max_ore) {
                make_ore_robot2(b, s, exp);
            }
            make_clay_robot2(b, s, exp);
            make_obsidian_robot2(b, s, exp);
            make_geode_robot2(b, s, exp);
        }
    }
    //std::cout << b.num << ": " << max_ore << "," << max_clay << " = " << best << " in " << finished << "\n";

    return best;
}

int sum_scores(std::vector<blueprint> &blueprints) {
    int total = 0;

    for(auto &b : blueprints) {
        int best = find_best(b, 6);
        std::cout << b.num << ": best is: " << best << "\n";
        total += best * b.num;
    }

    return total;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex pat = std::regex("Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian.");
    std::smatch res;

    std::vector<blueprint> blueprints;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            blueprints.emplace_back(std::stoi(res[1]), std::stoi(res[2]), std::stoi(res[3]), std::stoi(res[4]), std::stoi(res[5]), std::stoi(res[6]), std::stoi(res[7]));
        }
    }

    std::cout << "Part1: " << sum_scores(blueprints) << "\n";

    return 0;
}

/*

1: () best is: 2
2: () best is: 1
3: () best is: 0
4: () best is: 0
5: () best is: 1
6: () best is: 0
7: () best is: 2
8: () best is: 1
9: () best is: 6
10: () best is: 0
11: () best is: 2
12: () best is: 0
13: () best is: 3
14: () best is: 0
15: () best is: 0
16: () best is: 0
17: () best is: 0
18: () best is: 8
19: () best is: 5
20: () best is: 0
21: () best is: 0
22: () best is: 0
23: () best is: 9
24: () best is: 9
25: () best is: 3
26: () best is: 0
27: () best is: 1
28: () best is: 7
29: () best is: 0
30: () best is: 8
1346


*/