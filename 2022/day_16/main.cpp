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

class Valve {
    public:
    int id;
    int flow;
    std::vector<Link> dest;
};

int get_index(std::map<std::string,int> &names, std::vector<Valve> &valves, std::string name) {
    int id;
    if(names.contains(name)) {
        id = names[name];
    } else {
        id = valves.size();
        Valve v;
        v.flow = -1;
        v.id = id;
        names[name] = id;
        valves.push_back(v);
    }
    return id;   
}

void simplify(std::vector<Valve> &valves, int &current, int remove) {
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

struct State {
    int score;
    int time_left;
    int pos;
    std::vector<Valve> valves;
};

int release_the_pressure(std::vector<Valve> &valves, int start) {

    std::map<int,std::vector<State>> exp;
    int in_flight = 1;
    int added = 0;

    exp[0] = std::vector<State> {State(0, 30, start, valves)};

    int max_score = 0;

    while(exp.size() > 0) {
        auto lp = exp.extract(std::prev(exp.end()));

        for(auto &s : lp.mapped()) {
            in_flight--;

            if(s.time_left == 0 || (s.valves[s.pos].flow == 0 && s.valves[s.pos].dest.size() == 0)) {
                if(s.score > max_score) {
                    std::cout << "score:" << s.score << " in_flight:" << in_flight << " added:" << added << "\n";
                }
                max_score = std::max(max_score, s.score);
                //if(max_score == 1850) {
                //    return max_score;
                //}
                continue;
            }

            int left = std::accumulate(s.valves.begin(), s.valves.end(), 0, [](int a, const Valve &v) { return a + (v.flow > 0 ? v.flow : 0); });
            if(s.score + (left * s.time_left) < max_score) {
                continue;
            }

            if(s.valves[s.pos].flow > 0) {
                int new_score = lp.key() + (s.valves[s.pos].flow * (s.time_left-1));
                int new_index = new_score;
                if(!exp.contains(new_index)) {
                    exp[new_index] = std::vector<State>();
                }
                exp[new_index].emplace_back(new_score, s.time_left-1, s.pos, s.valves);
                exp[new_index].back().valves[s.pos].flow = 0;
                in_flight++;
                added++;
            }
            for(auto &l : s.valves[s.pos].dest) {
                int new_index = s.score;
                if(!exp.contains(new_index)) {
                    exp[new_index] = std::vector<State>();
                }
                exp[new_index].emplace_back(s.score, std::max(0, s.time_left - l.cost), l.id, s.valves);
                if(exp[new_index].back().valves[s.pos].flow == 0) {
                    simplify(exp[new_index].back().valves, exp[new_index].back().pos, s.pos);
                }
                in_flight++;
                added++;
            }
        }
    }

    return max_score;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex reg = std::regex("^Valve ([^ ]*) has flow rate=(\\d+); tunnel[s]* lead[s]* to valve[s]* (.*)$");
    std::smatch res;
    std::map<std::string,int> names;
    std::vector<Valve> valves;

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
        auto it = std::find_if(valves.begin(), valves.end(), [id](const Valve &v) { return v.id != id && v.flow == 0; });
        if(it == valves.end()) {
            break;
        }
        simplify(valves, id, std::distance(valves.begin(), it));
    }

    std::cout << "Part1: " << release_the_pressure(valves, id) << "\n";
    return 0;
}
