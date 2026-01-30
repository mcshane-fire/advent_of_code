#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <sstream>

struct instruction {
    std::string var = "";
    int mul = 0;
    int val = 0;
    std::string next = "";
};

struct workflow {
    std::vector<struct instruction> rules;
};

int count_accepted(std::map<std::string,struct workflow>& workflows, std::vector<std::map<std::string,int>>& parts) {
    int total = 0;

    for(auto p : parts) {
        std::string workflow = "in";
        while(true) {
            //std::cout << "Workflow " << workflow << "\n";
            for(auto r : workflows[workflow].rules) {
                if(r.mul == 0) {
                    workflow = r.next;
                    break;
                } else {
                    if(p[r.var] * r.mul > r.val * r.mul) {
                        //std::cout << "  Matched rule " << r.var << "=" << p[r.var] << " " << r.mul << " " << r.val << ", going to " << r.next << "\n";
                        workflow = r.next;
                        break;
                    }
                }
            }
            if(workflow == "R" || workflow == "A") {
                //std::cout << "Finished with " << workflow << "\n";
                if(workflow == "A") {
                    total += p["x"] + p["m"] + p["a"] + p["s"];
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

    std::map<std::string,struct workflow> workflows;
    std::vector<std::map<std::string,int>> parts;

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
                    workflow.rules.emplace_back(res2[1], res2[2] == ">" ? 1 : -1, std::stoi(res2[3]), res2[4]);
                } else {
                    //std::cout << "Jump(" << ins << ") ";
                    workflow.rules.emplace_back("", 0, 0, ins);
                }
            }
            workflows[res[1]] = workflow;
            //std::cout << "\n";
        }
    }
    while(std::getline(input, line)) {
        if(std::regex_match(line, res, part)) {
            //std::cout << res[1] << " " << res[2] << " " << res[3] << " " << res[4] << "\n";
            parts.push_back({{"x",std::stoi(res[1])}, {"m",std::stoi(res[2])}, {"a",std::stoi(res[3])}, {"s",std::stoi(res[4])}});
        }
    }

    std::cout << "Part1: " << count_accepted(workflows, parts) << "\n";
    //std::cout << "Part2: " << find_area(swapped_instructions) << "\n";

    return 0;
}


