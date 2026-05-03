#include <string>
#include <fstream>
#include <iostream>
#include <vector>

enum OpCode {
    NOP,
    ACC,
    JMP
};

struct Instruction {
    OpCode op;
    int val;

    Instruction(std::string line) {
        switch(line[0]) {
            case 'n': op = NOP; break;
            case 'a': op = ACC; break;
            case 'j': op = JMP; break;
        }
        val = std::stoi(line.substr(4));
    }
};

std::pair<int,bool> find_loop(std::vector<Instruction> &program) {
    std::vector<bool> visited(program.size()+1);
    int acc = 0;
    int pc = 0;
    visited[program.size()] = true;

    while(!visited[pc]) {
        visited[pc] = true;
        switch(program[pc].op) {
            case ACC: acc += program[pc].val; pc++; break;
            case JMP: pc += program[pc].val; break;
            case NOP: pc++; break;
        }
    }

    return {acc, pc == program.size()};
}

int find_termination(std::vector<Instruction> &program) {
    for(auto &ins : program) {
        OpCode cur = ACC;
        OpCode next = ACC;
        switch(ins.op) {
            case ACC: break;
            case NOP: cur = NOP; next = JMP; break;
            case JMP: cur = JMP; next = NOP; break;
        }
        if(cur != ACC) {
            ins.op = next;
            auto [v,b] = find_loop(program);
            if(b) {
                return v;
            }
            ins.op = cur;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<Instruction> program;

    while(std::getline(input, line)) {
        program.push_back(line);
    }

    std::cout << "Part1: " << find_loop(program).first << "\n";
    std::cout << "Part2: " << find_termination(program) << "\n";

    return 0;
}
