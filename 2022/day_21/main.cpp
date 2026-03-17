#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

enum OpType {
    OP_NONE,
    OP_ADD,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
};

class Monkey {
    public:

    Monkey() {}

    Monkey(int64_t a) {
        val = a;
    }

    Monkey(std::string a1, std::string oper, std::string a2) {
        arg1 = a1;
        arg2 = a2;
        switch(oper[0]) {
            case '+': op = OP_ADD; break;
            case '-': op = OP_MINUS; break;
            case '*': op = OP_MULTIPLY; break;
            case '/': op = OP_DIVIDE; break;
        }
        depend = 2;
    }

    std::vector<std::string> next;

    int64_t val = 0;

    int depend = 0;
    std::string arg1 = "";
    OpType op = OP_NONE;
    std::string arg2 = "";
};

int64_t known_val(std::map<std::string,Monkey> &all, std::string node) {
    return all[all[node].arg1].depend == 0 ? all[all[node].arg1].val : all[all[node].arg2].val;
}

std::pair<std::string,bool> unknown(std::map<std::string,Monkey> &all, std::string node) {
    bool first = all[all[node].arg1].depend != 0;
    return {first ? all[node].arg1 : all[node].arg2, first};
}

int64_t eval(std::map<std::string,Monkey> all, bool find_humn = false) {
    std::vector<std::string> exp;

    if(find_humn) {
        all["humn"].depend = 1;
    }

    for(auto p : all) {
        if(p.second.depend == 0) {
            exp.push_back(p.first);
        }
    }

    while(exp.size() > 0) {
        std::string m = exp.back();
        exp.pop_back();
    
        if(all[m].arg1 != "") {
            switch(all[m].op) {
                case OP_ADD: all[m].val = all[all[m].arg1].val + all[all[m].arg2].val; break;
                case OP_MINUS: all[m].val = all[all[m].arg1].val - all[all[m].arg2].val; break;
                case OP_MULTIPLY: all[m].val = all[all[m].arg1].val * all[all[m].arg2].val; break;
                case OP_DIVIDE: all[m].val = all[all[m].arg1].val / all[all[m].arg2].val; break;
            }
        }

        for(auto n : all[m].next) {
            if(--all[n].depend == 0) {
                exp.push_back(n);
            }          
        }
    }

    if(find_humn) {
        int64_t target = known_val(all, "root");
        auto head = unknown(all, "root");

        while(head.first != "humn") {
            int64_t val = known_val(all, head.first);
            auto next_head = unknown(all, head.first);

            switch(all[head.first].op) {
                case OP_ADD: target -= val; break;
                case OP_MINUS: target = next_head.second ? target + val : val - target; break;
                case OP_MULTIPLY: target /= val; break;
                case OP_DIVIDE: target = next_head.second ? val * target : val / target; break;
            }

            head = next_head;
        }
        return target;
    } else {
        return all["root"].val;
    }
} 

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex leaf = std::regex("^([a-z]+): (\\d+)$");
    std::regex oper = std::regex("^([a-z]+): ([a-z]+) ([\\+\\-\\*/]) ([a-z]+)$");
    std::smatch res;
    std::map<std::string,Monkey> all;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, leaf)) {
            all[res[1]] = Monkey(std::stoi(res[2]));
        } else if(std::regex_match(line, res, oper)) {
            all[res[1]] = Monkey(res[2], res[3], res[4]);
        }
    }

    for(auto m : all) {
        if(m.second.depend) {
            all[m.second.arg1].next.push_back(m.first);
            all[m.second.arg2].next.push_back(m.first);
        }
    }

    std::cout << "Part1: " << eval(all) << "\n";
    std::cout << "Part2: " << eval(all, true) << "\n";

    return 0;
}
