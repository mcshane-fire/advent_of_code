#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <numeric>

class Link {
    public:
    int id;
    int cost;

    Link(int _id, int _cost) : id(_id), cost(_cost) {}
    Link(int _id) : id(_id), cost(0) {}
    Link() : id(-1), cost(-3) {}

    bool operator==(const Link &p) const {
        return id == p.id;
    }
};

class Old_Valve {
    public:
    int id;
    int flow;
    std::vector<Link> dest;
};

class Valve {
    public:

    Valve(int _id, int _flow, u_int64_t _cost1, u_int64_t _cost2, u_int32_t _linked) :
            id(_id), flow(_flow), cost1(_cost1), cost2(_cost2), linked(_linked) {}

    bool connected_to(int id) {
        return linked & (1<<id);
    }

    int cost_to(int id) {
        return id >= 10 ? (cost2 >> ((id-10)*6)) & 0x3f : (cost1 >> (id*6)) & 0x3f;
    }

    void set_connection(int id, u_int64_t cost) {
        linked |= 1<<id;
        if(id >= 10) {
            cost2 |= (cost << ((id-10)*6));
        } else {
            cost1 |= (cost << (id*6));
        }
    }

    void clear_connection(int id) {
        linked &= ~(1<<id);
        if(id >= 10) {
            cost2 &= ~(0x3fLL << ((id-10)*6));
        } else {
            cost1 &= ~(0x3fLL << (id*6));
        }
    }

    int id;
    int flow;
    u_int64_t cost1;
    u_int64_t cost2;
    u_int32_t linked;
};

void simplify(std::vector<Valve> &valves, int &current, int remove, int max_cost) {
    for(int i=0; i<valves.size(); i++) {
        if(valves[i].connected_to(remove)) {
            int base_cost = valves[i].cost_to(remove);
            int bp = 0;
            while(valves[remove].linked >= (1<<bp)) {
                if(valves[remove].connected_to(bp)) {
                    if(bp != i) {
                        int new_cost = base_cost + valves[remove].cost_to(bp);
                        if(new_cost <= max_cost) {
                            if(valves[i].connected_to(bp)) {
                                int existing = valves[i].cost_to(bp);
                                if(new_cost < existing) {
                                    valves[i].clear_connection(bp);
                                    valves[i].set_connection(bp, new_cost);
                                }
                            } else {
                                valves[i].set_connection(bp, new_cost);
                            }
                        }
                    }
                }
                bp++;
            }
            
            valves[i].clear_connection(remove);
        }
    }

    valves[remove].flow = -2;
    valves[remove].linked = 0;
    valves[remove].cost1 = 0;
    valves[remove].cost2 = 0;
}

struct State {
    int time_left;
    int pos;
    std::vector<Valve> valves;
};

int release_the_pressure(std::vector<Valve> valves, int start, int time_left, bool spawn) {

    std::map<int,std::vector<State>> exp;
    int in_flight = 1;
    int added = 0;
    int maximum_depth = spawn ? valves.size()/2 : 0;

    exp[0] = std::vector<State> {State(time_left, start, valves)};

    if(valves[start].flow != 0) {
        std::cout << "Expecting first valve " << start << " to have no flow, instead found " << valves[start].flow << "\n";
        return -1;
    }

    int max_score = 0;

    while(exp.size() > 0) {
        auto lp = exp.extract(std::prev(exp.end()));

        for(auto &s : lp.mapped()) {
            in_flight--;

            for(auto &v : s.valves) {
                for(int i=0; i<10; i++) {
                    if((v.linked & (1<<i)) && ((v.cost1 >> (6*i)) & 0x3f) == 0) {
                        std::cout << i << " " << v.linked << " " << v.cost1 << " " << ((v.cost1 >> (6*i)) & 0x3f) << "\n";
                    } else if((v.linked & (1<<i)) == 0 && ((v.cost1 >> (6*i)) & 0x3f) != 0) {
                        std::cout << v.linked << " " << v.cost1 << "\n"; 
                    }
                }
            }

            if(spawn) {
                int left = std::count_if(s.valves.begin(), s.valves.end(), [](const Valve &v) { return v.flow >= 0; });
                if(left < maximum_depth) {
                    continue;
                }

                std::vector<Valve> spawn_valves = s.valves;
                if(s.pos != start) {
                    simplify(spawn_valves, start, s.pos, time_left);
                }
                int total_score = lp.key() + release_the_pressure(spawn_valves, start, time_left, false);
                if(total_score > max_score) {
                    std::cout << "combined score(" << left << "," << lp.key() << "):" << total_score << " in_flight:" << in_flight << " added:" << added << "\n";
                }
                max_score = std::max(max_score, total_score);
            } else {
                int left = std::accumulate(s.valves.begin(), s.valves.end(), 0, [](int a, const Valve &v) { return a + (v.flow > 0 ? v.flow : 0); });
                if(lp.key() + (left * s.time_left) < max_score) {
                    continue;
                }
            }

            int bp = 0;
            bool found_better = false;
            while(s.valves[s.pos].linked >= (1<<bp)) {
                if(s.valves[s.pos].connected_to(bp)) {
                    int cost = s.valves[s.pos].cost_to(bp) + 1;
                    if(cost < s.time_left) {
                        int new_score = lp.key() + (s.valves[bp].flow * (s.time_left - cost));
                        if(!exp.contains(new_score)) {
                            exp[new_score] = std::vector<State>();
                        }

                        exp[new_score].emplace_back(s.time_left - cost, bp, s.valves);
                        if(!spawn || s.pos != start) {
                            simplify(exp[new_score].back().valves, exp[new_score].back().pos, s.pos, spawn ? time_left : s.time_left - cost); 
                        }
                        in_flight++;
                        added++;
                        found_better = true;
                    }
                }
                bp++;
            }

            if(!spawn && !found_better) {
                //if(lp.key() > max_score) {
                    //std::cout << "score:" << lp.key() << " in_flight:" << in_flight << " added:" << added << "\n";
                //}
                max_score = std::max(max_score, lp.key());
                continue;
            }
        }
    }

    return max_score;
}

void old_simplify(std::vector<Old_Valve> &valves, int &current, int remove) {
    for(int i=0; i<valves.size(); i++) {
        auto dit = std::find(valves[i].dest.begin(), valves[i].dest.end(), remove);
        if(dit != valves[i].dest.end()) {
            int base_cost = (*dit).cost;
            for(auto &p : valves[remove].dest) {
                if(p.id != i) {
                    auto eit = std::find(valves[i].dest.begin(), valves[i].dest.end(), p.id);
                    int new_cost = base_cost + p.cost;
                    if(eit == valves[i].dest.end()) {
                        valves[i].dest.push_back({p.id, new_cost});
                    } else if(new_cost < (*eit).cost) {
                        (*eit).cost = new_cost;
                    }
                }
            }
            valves[i].dest.erase(std::find(valves[i].dest.begin(), valves[i].dest.end(), remove));
        }
    }

    for(int i=0; i<valves.size(); i++) {
        if(i > remove) {
            valves[i].id--;
        }
        for(auto &l : valves[i].dest) {
            if(l.id > remove) {
                l.id--;
            }
        }
    }

    valves.erase(valves.begin() + remove);
    if(current > remove) {
        current--;
    }
}

int get_index(std::map<std::string,int> &names, std::vector<Old_Valve> &valves, std::string name) {
    int id;
    if(names.contains(name)) {
        id = names[name];
    } else {
        id = valves.size();
        names[name] = id;
        valves.emplace_back(id, -1);
    }
    return id;
}

void find_paths(std::vector<Old_Valve> &valves) {
    for(int start=0; start<valves.size(); start++) {
        std::map<int,std::vector<int>> exp;

        for(auto &l : valves[start].dest) {
            exp[l.cost].push_back(l.id);
        }

        valves[start].dest.resize(0);

        while(exp.size() > 0) {
            auto le = exp.extract(exp.begin());

            for(int d : le.mapped()) {
                auto it = std::find(valves[start].dest.begin(), valves[start].dest.end(), d);
                if(it == valves[start].dest.end()) {
                    valves[start].dest.emplace_back(d, le.key());
                    for(auto &l : valves[d].dest) {
                        if(l.id != start && std::find(valves[start].dest.begin(), valves[start].dest.end(), l.id) == valves[start].dest.end()) {
                            exp[le.key() + l.cost].push_back(l.id);
                        }
                    }
                }         
            }
        }
    }
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex reg = std::regex("^Valve ([^ ]*) has flow rate=(\\d+); tunnel[s]* lead[s]* to valve[s]* (.*)$");
    std::smatch res;
    std::map<std::string,int> names;
    std::vector<Old_Valve> valves;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, reg)) {
            int id = get_index(names, valves, res[1]);
            valves[id].flow = std::stoi(res[2]);
            std::string s = res[3];
            size_t pos = 0;
            while((pos = s.find(',')) != std::string::npos) {
                int did = get_index(names, valves, s.substr(0,pos));
                valves[id].dest.push_back({did, 1});
                s = s.substr(pos+2);
            }
            int nid = get_index(names, valves, s);
            valves[id].dest.push_back({nid, 1});
        }
    }

    int id = get_index(names, valves, "AA");
    while(true) {
        auto it = std::find_if(valves.begin(), valves.end(), [id](const Old_Valve &v) { return v.id != id && v.flow == 0; });
        if(it == valves.end()) {
            break;
        }
        old_simplify(valves, id, std::distance(valves.begin(), it));
    }

    if(valves.size() > 20) {
        std::cout << "Problem too large\n";
        return 0;
    }

    find_paths(valves);

    std::vector<Valve> stripped_valves;
    for(auto &v : valves) {
        stripped_valves.emplace_back(v.id, v.flow, 0, 0, 0);

        for(auto &l : v.dest) {
            stripped_valves.back().set_connection(l.id, l.cost);
        }
    }

    std::cout << "Part1: " << release_the_pressure(stripped_valves, id, 30, false) << "\n";
    std::cout << "Part2: " << release_the_pressure(stripped_valves, id, 26, true) << "\n";
    return 0;
}
