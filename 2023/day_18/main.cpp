#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>

enum direction {
    RIGHT,
    UP,
    LEFT,
    DOWN,
};

struct instruction {
    direction dir;
    int len;
};

struct range {
    int start;
    int len;
};

void add(std::map<std::pair<int,int>,std::set<std::pair<int,int>>>& edges, int x, int y, int len, direction dir, bool corner) {
    //std::cout << " Add x:" << x << " y:" << y << " Len:" << len << " Dir:" << dir << " Corner:" << corner << "\n";
    std::pair<int,int> rec(x, (dir == UP ? 1 : 0) | (corner ? 2 : 0));

    int y_min = std::min(y, y + ((len-1) * (dir == UP ? -1 : 1)));
    //int y_max = std::max(y, y + ((len) * (dir == UP ? -1 : 1)));
    
    //std::cout << "  Adding interval y_min:" <<y_min<<" len:" << len << "\n";
    while(len > 0) {
        bool added = false;
        for(auto p : edges) {
            //std::cout << "    r: start:" << p.first.first << " len:" << p.first.second << "\n";
            if(y_min < p.first.first) {
                int u_len = std::min(len, p.first.first - y_min);
                std::pair<int,int> r = {y_min,u_len};
                edges[r] = std::set<std::pair<int,int>>();
                edges[r].insert(rec);
                y_min += u_len;
                len -= u_len;
                //std::cout << "    - add before first, used:" << u_len << ", " << len << " remaining\n";
                added = true;
                break;
            } else if(y_min == p.first.first) {
                if(len >= p.first.second) {
                    edges[p.first].insert(rec);
                    len -= p.first.second;
                    y_min += p.first.second;
                    //std::cout << "    - added to existing range, used: " << p.first.second << ", " << len << "  remaining\n";
                    if(len == 0) {
                        break;
                    }
                } else {
                    std::pair<int,int> r = {y_min,len};
                    std::pair<int,int> r2 = {y_min+len,p.first.second-len};

                    edges[r] = p.second;
                    edges[r].insert(rec);
                    edges[r2] = p.second;
                    //std::cout << "    - split range from start to " << r.second << "/" << r2.second << "\n";
                    edges.erase(p.first);
                    len = 0;
                    break;
                }
            } else if(y_min > p.first.first && y_min < p.first.first + p.first.second) {
                int u_len = std::min(len, p.first.first + p.first.second - y_min);
                std::pair<int,int> r = {p.first.first, y_min-p.first.first};
                std::pair<int,int> r2 = {y_min, u_len};
                std::pair<int,int> r3 = {y_min+u_len, p.first.first + p.first.second - y_min - u_len};

                edges[r] = p.second;
                edges[r2] = p.second;
                edges[r2].insert(rec);
                if(r3.second > 0) {
                    edges[r3] = p.second;
                }
                //std::cout << "    - split range from middle into " << r.second << "/" << r2.second << "/" << r3.second << "\n";
                len -= u_len;
                y_min += u_len;
                added = true;
                edges.erase(p.first);
                break;
            }
        }

        if(len > 0 && !added) {
            //std::cout << "    - adding new range at end\n";
            std::pair<int,int> r = {y_min, len};
            edges[r] = std::set<std::pair<int,int>>();
            edges[r].insert(rec);
            len = 0;
        }
    }
    /*
    int last = (*edges.begin()).first.first;
    for(auto p : edges) {
        if(last != p.first.first) {
            std::cout << "   error: Discontinuity " << last << " vs. " << p.first.first << "\n";
        }
        if(p.first.second == 0) {
            std::cout << "   error: Zero length\n";
        }
        std::cout << "  Edge " << p.first.first << "," << p.first.second << " : ";
        last = p.first.first + p.first.second;
        for(auto p2 : p.second) {
            std::cout << p2.first << "," << p2.second << " ";
        }
        std::cout << "\n";
    }*/
}

int64_t find_area(std::vector<struct instruction>& instructions) {
    int64_t total = 0;
    std::map<std::pair<int,int>,std::set<std::pair<int,int>>> edges;

    std::map<direction,int> xd = {{UP,0}, {DOWN,0}, {LEFT,-1}, {RIGHT,1}};
    std::map<direction,int> yd = {{UP,-1}, {DOWN,1}, {LEFT,0}, {RIGHT,0}};

    std::map<int,char> pchar = {{0,'v'}, {1,'^'}, {2,'V'}, {3,'A'}};

    int x_min = 0, x_max = 0;

    int x=0, y=0;
    for(auto ins : instructions) {
        //std::cout << "Instruction Dir:" << ins.dir << " Len:" << ins.len << " from " <<x<<","<<y<<", total:" << total << "\n";

        if(xd[ins.dir] == 0) {
            add(edges, x, y, 1, ins.dir, true);
            if(ins.len > 1) {
                add(edges, x, y+yd[ins.dir], ins.len-1, ins.dir, false);
            }
            y += yd[ins.dir] * ins.len;
            add(edges, x, y, 1, ins.dir, true);
        } else {
            x += xd[ins.dir] * ins.len;
        }
        total += ins.len;
        /*
        for(int i=0; i<ins.len; i++) {
            x += xd[ins.dir];
            y += yd[ins.dir];
            add(edges, x, y, ins.dir, i == ins.len-1);
            total++;
        }*/
        x_min = std::min(x, x_min);
        x_max = std::max(x, x_max);
    }

    //std::cout << "perimeter (" << total << ")\n";

    
    for(auto ep : edges) {
        //std::string line = std::string(x_max + 1 - x_min, '.');
        bool inside = false;
        for(auto it = ep.second.begin(); it != ep.second.end(); ) {
            auto p = *it;
            auto nit = ++it;
            //line[p.first - x_min] = pchar[p.second];

            if(p.second & 2) {
                if(((p.second ^ (*nit).second) & 1) == 0) {
                    inside = !inside;
                }
                p = *nit;
                nit = ++it;
                //line[p.first - x_min] = pchar[p.second];
            } else {
                inside = !inside;
            }
            if(inside) {
                total += (int64_t) ep.first.second * (int64_t) ((*nit).first - p.first - 1);
                //for(int i=p.first+1; i<(*nit).first; i++) {
                    //line[i - x_min] = '#';
                    //total += ep.first.second;
                //}
            }

        }
        //std::cout << line << "   * " << ep.first.second << "\n";
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line, part;
    std::regex pat = std::regex("^([UDLR]) (\\d+) \\(#([0-9a-f]*)([0-3])\\)$"); 
    std::map<std::string,direction> dmap = {{"U",UP}, {"D",DOWN}, {"L",LEFT}, {"R",RIGHT},
                                            {"0",RIGHT}, {"1",DOWN}, {"2",LEFT}, {"3",UP}};
    std::smatch res;

    std::vector<struct instruction> instructions;
    std::vector<struct instruction> swapped_instructions;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            //std::cout << res[1] << " " << res[2] << " " << res[3] << "\n";
            instructions.emplace_back(dmap[res[1]], std::stoi(res[2]));
            int num = 0;
            std::string res3 = std::string(res[3]);
            for(int i=0; i<res3.length(); i++) {
                if(res3[i] >= '0' && res3[i] <= '9') {
                    num = (num<<4) + res3[i]-'0';
                } else if(res3[i] >= 'a' && res3[i] <= 'f') {
                    num = (num<<4) + (res3[i] - 'a' + 10);
                }
            }
            swapped_instructions.emplace_back(dmap[res[4]], num);
        }
    }

    std::cout << "Part1: " << find_area(instructions) << "\n";
    std::cout << "Part2: " << find_area(swapped_instructions) << "\n";
    return 0;
}


