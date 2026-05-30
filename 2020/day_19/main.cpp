#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

struct Rule {
    int num;
    char letter;
    bool is_letter;
    std::vector<std::vector<int>> var;

    Rule() : num(0), letter(' '), is_letter(true) {};
    Rule(std::string line) : num(0), letter(' '), is_letter(false) {
        size_t pos;
        num = std::stoi(line, &pos);

        line = line.substr(pos+2);
        if(line[0] == '"') {
            is_letter = true;
            letter = line[1];
        } else {
            var.push_back(std::vector<int>());
            while(line.length() > 0) {
                if(line[0] == ' ') {
                    line = line.substr(1);
                }
                if(line[0] == '|') {
                    var.push_back(std::vector<int>());
                    line = line.substr(2);
                }
                var.back().push_back(std::stoi(line, &pos));
                line = line.substr(pos);
            }
        }
    }
};

void print(std::vector<Rule> &rules, int num, int indent = 0) {
    std::cout << std::string(indent, ' ');
    if(rules[num].is_letter) {
        std::cout << "Rule " << num << " = " << rules[num].letter << "\n";
    }
    else {
        std::cout << "Rule " << num << " = ";
        std::set<int> dep;
        for(auto &v : rules[num].var) {
            for(auto n : v) {
                std::cout << n << " ";
                dep.insert(n);
            }
            std::cout << "| ";
        }
        std::cout << "\n";
        for(int n : dep) {
            print(rules, n, indent + 3);
        }
    }
}

void generate_all(std::vector<Rule> &rules, int num, std::vector<std::set<std::string>> &all) {
    if(all[num].size() > 0) {
        return;
    } else if(rules[num].is_letter) {
        all[num].insert(std::string(1, rules[num].letter));
    } else {
        for(auto &v : rules[num].var) {
            generate_all(rules, v[0], all);
            std::set<std::string> base = all[v[0]];
            for(int i=1; i<v.size(); i++) {
                std::set<std::string> agg;
 
                generate_all(rules, v[i], all);
                for(auto &s1 : base) {
                    for(auto &s2 : all[v[i]]) {
                        agg.insert(s1 + s2);
                    }
                }
                base.clear();
                base.swap(agg);
            }
            for(auto &s : base) {
                all[num].insert(s);
            }
        }
    }
}

int count_matches(std::vector<Rule> &rules, int num, std::vector<std::string> &list) {
    auto all = std::vector<std::set<std::string>>(rules.size());
    generate_all(rules, num, all);
    int count = 0;
    for(auto &s : list) {
        if(std::find(all[num].begin(), all[num].end(), s) != all[num].end()) {
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Rule> rules;
    std::vector<std::string> list;

    while(std::getline(input, line) && line.length() > 0) {
        Rule r = Rule(line);
        if(r.num >= rules.size()) {
            rules.resize(r.num+1);
        }
        rules[r.num] = r;
    }

    while(std::getline(input, line)) {
        list.push_back(line);
    }

    std::cout << "Part1: " << count_matches(rules, 0, list) << "\n";

    return 0;
}
