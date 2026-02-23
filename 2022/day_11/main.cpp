#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

struct monkey {
    std::vector<int64_t> items;
    bool operation_add;
    int operation_num;
    int test_div;
    int true_monkey;
    int false_monkey;
    int64_t inspects = 0;
};

void do_round(std::vector<monkey> &monkeys, int modulo) {
    for(auto &m : monkeys) {
        for(int64_t i : m.items) {
            m.inspects++;
            i = m.operation_add ? i + m.operation_num : (m.operation_num ? i * m.operation_num : i * i);
            if(i < 0) {
                std::cout << i;
            }
            if(modulo) {
                i = i % modulo;
            } else {
                i = i/3;
            }
            monkeys[(i % m.test_div) ? m.false_monkey : m.true_monkey].items.push_back(i);
        }
        m.items = std::vector<int64_t>();
    }
}

int64_t monkey_business(std::vector<monkey> monkeys, int rounds, bool div3) {
    int lcm = std::lcm(monkeys[0].test_div, monkeys[1].test_div);
    for(int i=2; i<monkeys.size(); i++) {
        lcm = std::lcm(lcm, monkeys[i].test_div);
    }

    for(int i=0; i<rounds; i++) {
        do_round(monkeys, div3 ? 0 : lcm);
    }

    std::sort(monkeys.begin(), monkeys.end(), [](auto m1, auto m2) {return m1.inspects > m2.inspects; });
    return monkeys[0].inspects * monkeys[1].inspects;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<monkey> monkeys;

    while(std::getline(input, line)) {
        if(line == "") {
            continue;
        }
        if(line[0] == 'M') {
            struct monkey m;
            monkeys.push_back(m);
        } else if(line[2] == 'S') {
            for(int i=18; i<line.length(); i+=4) {
                monkeys.back().items.push_back(std::stoi(line.substr(i)));
            }
        } else if(line[2] == 'O') {
            monkeys.back().operation_add = line[23] == '+';
            if(line.substr(25) == "old") {
                monkeys.back().operation_num = 0;
            } else {
                monkeys.back().operation_num = std::stoi(line.substr(25));
            }
        } else if(line[2] == 'T') {
            monkeys.back().test_div = std::stoi(line.substr(21));
        } else if(line[7] == 't') {
            monkeys.back().true_monkey = std::stoi(line.substr(28));
        } else if(line[7] == 'f') {
            monkeys.back().false_monkey = std::stoi(line.substr(29));
        }
    }    

    std::cout << "Part1: " << monkey_business(monkeys, 20, true) << "\n";
    std::cout << "Part2: " << monkey_business(monkeys, 10000, false) << "\n";

    return 0;
}