#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <numeric>

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

int mlen(std::set<std::string> &list) {
    return std::accumulate(list.begin(), list.end(), 0, [](int m, auto &s) { return std::max(m, (int) s.length()); });
}

void generate_all(std::vector<Rule> &rules, int num, int max_len, std::vector<std::set<std::string>> &all, int indent = 0) {
    if(max_len <= 0) {
        //std::cout << std::string(indent, ' ') << "Exit from generating rule " << num << ", max_len = " << max_len << "\n";
        return;
    }
    //std::cout << std::string(indent, ' ') << "Generate rule " << num << ", max_len = " << max_len << "\n";
    if(all[num].size() > 0) {
        //std::cout << std::string(indent, ' ') << " - return cached set of " << all[num].size() << " max " << mlen(all[num]) << "\n";
        return;
    }
    if(rules[num].is_letter) {
        all[num].insert(std::string(1, rules[num].letter));
    } else {
        for(auto &v : rules[num].var) {
            //std::cout << std::string(indent, ' ') << " - new variation, starts with " << v[0] << "\n";
            generate_all(rules, v[0], max_len-1, all, indent+3);
            std::set<std::string> base = all[v[0]];

            for(int i=1; i<v.size(); i++) {
                std::set<std::string> agg;

                //std::cout << std::string(indent, ' ') << " - continues with " << v[i] << "\n";
                generate_all(rules, v[i], max_len-1, all, indent+3);
                
                for(auto &s1 : base) {
                    for(auto &s2 : all[v[1]]) {
                        agg.insert(s1 + s2);
                    }
                }
                //std::cout << std::string(indent, ' ') << " - up to " << agg.size() << " patterns\n";
                base.clear();
                base.swap(agg);
            }
            for(auto &s : base) {
                if(s.length() <= max_len) {
                    all[num].insert(s);
                }
            }
            //std::cout << std::string(indent, ' ') << " - completed variant for rule " << num << ", added " << all[num].size() << " from " << base.size() << " patterns, max " << mlen(all[num]) << "\n";
        }
        //std::cout << std::string(indent, ' ') << " - all done rule " << num << " with " << all[num].size() << " patterns, max " << mlen(all[num]) << "\n";
    }
}

void print_list(std::vector<std::set<std::string>> &all, int num) {
    std::cout << "Rule " << num << ": " << all[num].size() << " patterns, max " << mlen(all[num]) << "\n";
}

int check_repeated(const std::string &line, std::set<std::string> &list) {
    int len = (*list.begin()).length();
    if(line.length() % len != 0) {
        return 0;
    }
    for(int i=0; i<line.length(); i+=len) {
        if(std::find(list.begin(), list.end(), line.substr(i, len)) == list.end()) {
            return 0;
        }
    }
    return line.length() / len;
}

std::pair<int, int> count_matches(std::vector<Rule> &rules, int num, std::vector<std::string> &list) {
    auto all = std::vector<std::set<std::string>>(rules.size());
    int max_len = std::accumulate(list.begin(), list.end(), 0, [](int m, auto &s) { return std::max(m, (int) s.length()); });
    generate_all(rules, num, max_len, all);
    int count = 0;
    for(auto &s : list) {
        if(std::find(all[num].begin(), all[num].end(), s) != all[num].end()) {
            count++;
        }
    }

    int part2 = 0;
    // assumes we're matching "42+ 31+" with more 42s than 31s

    int len_42 = (*all[42].begin()).length();
    int len_31 = (*all[31].begin()).length();
    for(auto &s : list) {
        int found_42 = 0;
        while((found_42 * len_42) <= s.length() - len_42) {
            if(std::find(all[42].begin(), all[42].end(), s.substr(found_42 * len_42, len_42)) != all[42].end()) {
                found_42++;
                int found_31 = check_repeated(s.substr(found_42 * len_42), all[31]);
                if(found_31 > 0 && found_31 < found_42) {
                    part2++;
                    break;
                }
            } else {
                break;
            }
        }
    }

    return {count, part2};
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input2.txt";
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

    auto p = count_matches(rules, 0, list);

    std::cout << "Part1: " << p.first << "\n";
    std::cout << "Part2: " << p.second << "\n";

    return 0;
}
