#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <sstream>

enum component {
    COMP_X = 0,
    COMP_M = 1,
    COMP_A = 2,
    COMP_S = 3,
};

struct range {
    int min;
    int max;

    friend std::ostream& operator<<(std::ostream& os, const struct range& r) {
        os << "[" << r.min << ".." << r.max << "]";
        return os;
    }

    int64_t size() {
        if(max >= min) {
            return max-min+1;
        } else {
            return 0;
        }
    }
};

struct accepted_range {
    public:
    accepted_range() {
        r[COMP_X] = {1,4000};
        r[COMP_M] = {1,4000};
        r[COMP_A] = {1,4000};
        r[COMP_S] = {1,4000};
    }

    friend std::ostream& operator<<(std::ostream& os, const accepted_range& ar) {
        os << "AR(x=" << ar.r[COMP_X] << " m=" << ar.r[COMP_M] << " a=" << ar.r[COMP_A] << " s=" << ar.r[COMP_S] << ")";
        return os;
    }

    void combine(struct range& nr, enum component comp) {
        //std::cout << "combine " << r[comp] << " with " << nr;
        r[comp].min = std::max(r[comp].min, nr.min);
        r[comp].max = std::min(r[comp].max, nr.max);
        //std::cout << " to get " << r[comp] << "\n";
    }

    int64_t size() {
        return r[0].size() * r[1].size() * r[2].size() * r[3].size();
    }

    private:
    struct range r[4];
};

struct rule {
    enum component var;
    int mul = 0;
    int val = 0;
    std::string next = "";

    friend std::ostream& operator<< (std::ostream& stream, const struct rule &r) {
        if(r.mul == 0) {
            stream << "->" << r.next << ",";
        } else {
            char cc[] = {'x', 'm', 'a', 's'};
            stream << "if(" << cc[r.var] << (r.mul == 1 ? ">" : "<") << r.val << ")->" << r.next << ",";
        } 
        return stream;
    }

    void apply(std::vector<accepted_range>& base, const std::vector<accepted_range>& branch) {
        //std::cout << "apply" << *this << " to len " << branch.size() << "\n";

        struct range branch_range(mul == 1 ? val+1 : 1, mul == 1 ? 4000 : val-1);
        struct range base_range(mul == 1 ? 1 : val, mul == 1 ? val : 4000);

        for(auto& ar : base) {
            ar.combine(base_range, var);
        }

        for(auto& ar : branch) {
            accepted_range nar = ar;
            nar.combine(branch_range, var);
            base.push_back(nar);
        }
    }
};

struct part {
    public:
    part(int x, int m, int a, int s) {
        val[COMP_X] = x;
        val[COMP_M] = m;
        val[COMP_A] = a;
        val[COMP_S] = s;
    }

    int total() {
        return val[0] + val[1] + val[2] + val[3];
    }

    int val[4];
};

struct workflow {
    std::vector<struct rule> rules;
    std::set<std::string> downstream;
    std::set<std::string> upstream;
};

int64_t calculate_number_accepted(std::map<std::string,struct workflow>& workflows) {
    std::map<std::string,std::vector<accepted_range>> valid;

    valid["A"] = std::vector<accepted_range>({accepted_range()});
    valid["R"] = std::vector<accepted_range>();

    while(!valid.contains("in")) {
        for(auto& p : workflows) {
            if(p.second.downstream.size() == 0 && !valid.contains(p.first)) {
                //std::cout << "Zero downstream dependencies for " << p.first << "\n";

                std::vector<accepted_range> cur = valid[(*p.second.rules.rbegin()).next];
                //std::cout << "starting with ranges from " << (*p.second.rules.rbegin()).next << ":\n";
                //for(auto& ar : cur) {
                //    std::cout << "   " << ar << "\n";
                //}

                for(auto it = p.second.rules.rbegin()+1; it != p.second.rules.rend(); it++) {
                    //std::cout << "Applying rule: " << (*it) << "\n";
                    (*it).apply(cur, valid[(*it).next]);
                    //for(auto& ar : cur) {
                    //    std::cout << "   " << ar << "\n";
                    //}
                }

                valid[p.first] = cur;
                for(auto& w : p.second.upstream) {
                    //std::cout << " Removed downstream dependency at " << w << "\n";
                    workflows[w].downstream.erase(p.first);
                }

                //std::cout << "\n";
            }
        }
    }

    int64_t total = 0;
    for(auto& ar : valid["in"]) {
        total += ar.size();
    }

    return total;
}

int count_accepted(std::map<std::string,struct workflow>& workflows, std::vector<struct part>& parts) {
    int total = 0;

    for(auto& p : parts) {
        std::string workflow = "in";
        while(true) {
            //std::cout << "Workflow " << workflow << "\n";
            for(auto r : workflows[workflow].rules) {
                if(r.mul == 0) {
                    workflow = r.next;
                    break;
                } else {
                    if(p.val[r.var] * r.mul > r.val * r.mul) {
                        //std::cout << "  Matched rule " << r.var << "=" << p[r.var] << " " << r.mul << " " << r.val << ", going to " << r.next << "\n";
                        workflow = r.next;
                        break;
                    }
                }
            }
            if(workflow == "R" || workflow == "A") {
                //std::cout << "Finished with " << workflow << "\n";
                if(workflow == "A") {
                    total += p.total();
                }
                break;
            }
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::regex rule = std::regex("^([a-z]*)\\{(.*)\\}$");
    std::regex test = std::regex("^([xmas])([<>])(\\d+):([A-Za-z]+)$");
    std::regex part = std::regex("^\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}$");
    std::smatch res;
    std::map<std::string,enum component> comp_map = {{"x",COMP_X}, {"m",COMP_M}, {"a",COMP_A}, {"s",COMP_S}};

    std::map<std::string,struct workflow> workflows;
    std::vector<struct part> parts;

    while(std::getline(input, line)) {
        if(line.length()==0) {
            break;
        }
        if(std::regex_match(line, res, rule)) {
            std::stringstream ss(res[2]);
            std::string ins;
            //std::cout << res[1] << ":";
            struct workflow workflow;
            while(getline(ss, ins, ',')) {
                std::smatch res2;
                if(std::regex_match(ins, res2, test)) {
                    //std::cout << ins << "->Test(" << res2[1] << "," << res2[2] << "," << res2[3] << "," << res2[4] << ") ";
                    workflow.rules.emplace_back(comp_map[res2[1]], res2[2] == ">" ? 1 : -1, std::stoi(res2[3]), res2[4]);
                } else {
                    //std::cout << "Jump(" << ins << ") ";
                    workflow.rules.emplace_back(COMP_X, 0, 0, ins);
                }
            }
            workflows[res[1]] = workflow;
            //std::cout << "\n";
        }
    }
    while(std::getline(input, line)) {
        if(std::regex_match(line, res, part)) {
            //std::cout << res[1] << " " << res[2] << " " << res[3] << " " << res[4] << "\n";
            parts.push_back({std::stoi(res[1]), std::stoi(res[2]), std::stoi(res[3]), std::stoi(res[4])});
        }
    }

    for(auto& w : workflows) {
        for(auto& r : w.second.rules) {
            if(r.next != "R" && r.next != "A") {
                workflows[r.next].upstream.insert(w.first);
                w.second.downstream.insert(r.next);
            }
        }
    }

    std::cout << "Part1: " << count_accepted(workflows, parts) << "\n";
    std::cout << "Part2: " << calculate_number_accepted(workflows) << "\n";

    return 0;
}


