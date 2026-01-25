#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>

struct instruction {
    std::string dir;
    int len;
    std::string colour;
};

void add(std::map<int,std::set<std::pair<int,int>>>& edges, int x, int y, std::string dir, bool corner) {
    if(dir == "U" || dir == "D") {
        if(!edges.contains(y)) {
            edges[y] = std::set<std::pair<int,int>>();
        }
        edges[y].insert({x, (dir == "U" ? 1 : 0) | (corner ? 2 : 0)});
    }
}

int find_area(std::vector<struct instruction>& instructions) {
    int total = 0;
    std::map<int,std::set<std::pair<int,int>>> edges;

    std::map<std::string,int> xd = {{"U",0}, {"D",0}, {"L",-1}, {"R",1}};
    std::map<std::string,int> yd = {{"U",-1}, {"D",1}, {"L",0}, {"R",0}};

    std::map<int,char> pchar = {{0,'v'}, {1,'^'}, {2,'V'}, {3,'A'}};

    int x_min = 0, x_max = 0;

    int x=0, y=0;
    for(auto ins : instructions) {
        add(edges, x, y, ins.dir, true);
        for(int i=0; i<ins.len; i++) {
            x += xd[ins.dir];
            y += yd[ins.dir];
            add(edges, x, y, ins.dir, i == ins.len-1);
            total++;
        }
        x_min = std::min(x, x_min);
        x_max = std::max(x, x_max);
    }

    std::cout << "perimeter (" << total << ")";
    
    for(auto p : edges) {
        std::string line = std::string(x_max + 1 - x_min, '.');
        bool inside = false;
        for(auto it = p.second.begin(); it != p.second.end(); ) {
            auto p = *it;
            auto nit = ++it;
            line[p.first - x_min] = pchar[p.second];

            if(p.second & 2) {
                if(((p.second ^ (*nit).second) & 1) == 0) {
                    inside = !inside;
                }
                p = *nit;
                nit = ++it;
                line[p.first - x_min] = pchar[p.second];
            } else {
                inside = !inside;
            }
            if(inside) {
                for(int i=p.first+1; i<(*nit).first; i++) {
                    line[i - x_min] = '#';
                    total++;
                }
            }

        }
        std::cout << line << "\n";
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line, part;
    std::regex pat = std::regex("^([UDLR]) (\\d+) \\(#([0-9a-f]*)\\)$");
    std::smatch res;

    std::vector<struct instruction> instructions;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            //std::cout << res[1] << " " << res[2] << " " << res[3] << "\n";
            instructions.emplace_back(res[1], std::stoi(res[2]), res[3]);
        }
    }

    std::cout << "Part1: " << find_area(instructions) << "\n";
    return 0;
}


