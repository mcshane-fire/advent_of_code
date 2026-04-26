#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

enum OpCode {
    NOP,
    INP,
    ADD,
    MUL,
    DIV,
    MOD,
    EQL,
};

struct Instruction {
    OpCode op = NOP;
    int src = -1;
    bool is_var = false;
    int num = 0;
    int dest = -1;

    Instruction(std::string line) {
        std::map<std::string,OpCode> dop = {{"inp", INP}, {"add", ADD}, {"mul", MUL}, {"div", DIV}, {"mod", MOD}, {"eql", EQL}};
        std::map<char,int> dvar = {{'w', 0}, {'x', 1}, {'y', 2}, {'z', 3}};

        op = dop[line.substr(0, 3)];
        src = dvar[line[4]];
        if(op != INP) {
            if(dvar.contains(line[6])) {
                is_var = true;
                dest = dvar[line[6]]; 
            } else {
                is_var = false;
                num = std::stoi(line.substr(6));
            }
        }
    }
};

struct State {
    int var[4] = {0, 0, 0, 0};
    std::vector<int> input;

    void apply(const Instruction &ins) {
        switch(ins.op) {
            case INP:
                var[ins.src] = input.back();
                input.pop_back();
                break;
            case ADD:
                var[ins.src] += ins.is_var ? var[ins.dest] : ins.num;
                break;
            case MUL:
                var[ins.src] *= ins.is_var ? var[ins.dest] : ins.num;
                break;
            case DIV:
                var[ins.src] /= ins.is_var ? var[ins.dest] : ins.num;
                break;
            case MOD:
                var[ins.src] %= ins.is_var ? var[ins.dest] : ins.num;
                break;
            case EQL:
                var[ins.src] = var[ins.src] == (ins.is_var ? var[ins.dest] : ins.num) ? 1 : 0;
                break;
        }
    }

    friend std::ostream& operator<<(std::ostream &os, const State &st) {
        int t = st.var[3];
        os << "z:";
        while(t > 0) {
            os << (t % 26) << "|";
            t /= 26;
        }
        os << " (" << st.var[0] << ":" << st.var[1] << ":" << st.var[2] << ")";
        return os;
    }    

};

bool run_program(std::vector<Instruction> &instructions, std::string init) {
    State st;

    for(auto c : init) {
        st.input.insert(st.input.begin(), c - '0');
    }

    int count = 0;
    int number = 1;
    for(auto &i : instructions) {
        if(count == 0) {
            std::cout << number << ": " << st.input.back() << " -> ";
            number++;
        }

        st.apply(i);
        if(++count == 18) {
            std::cout << st << "\n";
            count = 0;
        }
    }

    return st.var[3] == 0;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<Instruction> instructions;

    while(std::getline(input, line)) {
        instructions.push_back(line);
    }

    std::cout << "Part1:" << (run_program(instructions, "96299896449997") ? "complete" : "fail") << "\n";
    std::cout << "Part2:" << (run_program(instructions, "31162141116841") ? "complete" : "fail") << "\n";

    return 0;
}
