#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

bool impossible(std::string inf, std::vector<int>& spec, int& total) {
    std::regex group = std::regex("^(\\.*)(#+)(\\.|$)");
    std::smatch res;
    int found = 0;

    //std::cout << "impossible " << inf << "\n";

    while(std::regex_search(inf, res, group)) {
        //std::cout << " -> Looking at " << inf << ", found group: " << res[2] << ", looking for " << spec[found] << "\n";
        if(found > spec.size() || spec[found] != res[2].length()) {
            //std::cout << "    - stopping now\n";
            return true;
        }
        found++;
        inf = inf.substr(res[1].length() + res[2].length());
        std::string dots = std::string(inf.length(),'.');
        if(inf.length() == 0 || inf == dots) {
            if(spec.size() != found) {
                //std::cout << "   - reached end of string with " << (spec.size() - found) << " groups left to find, stopping now\n";
                return true;
            } else {
                //std::cout << "    - think this works\n";
                total++;
            }
            break;
        }
        //std::cout << "  -> will next look at " << inf <<  "\n";
    }

    return false;
}

void try_solution(std::string inf, std::vector<int>& spec, int start, int& total) {
    while(start < inf.length() && inf[start] != '?') {
        start++;
    }

    if(start >= inf.length()) {
        //std::cout << "Got to end of string: " << inf << "\n";
        return;
    }

    inf[start] = '.';
    if(!impossible(inf, spec, total)) {
        try_solution(inf, spec, start+1, total);
    }
    inf[start] = '#';
    if(!impossible(inf, spec, total)) {
        try_solution(inf, spec, start+1, total);
    }
}

int sum_solutions(std::vector<std::pair<std::string,std::vector<int>>>& problems)
{
    int total = 0;

    for(auto p : problems) {
        /*std::cout << "\n\nProblem: " << p.first << ": ";
        for(auto i : p.second) {
            std::cout << i << "-";
        }
        std::cout << "\n";
        */

        try_solution(p.first, p.second, 0, total);
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::pair<std::string,std::vector<int>>> problems;

    while(std::getline(input, line)) {
        std::stringstream ss(line);
        std::string inf, seq, num;
        std::vector<int> nums;
        ss >> inf;
        ss >> seq;
        ss = std::stringstream(seq);
        while(getline(ss, num, ',')) {
            nums.push_back(std::stoi(num));
        }
        problems.emplace_back(inf, nums);
    }

    std::cout << "Part1: " << sum_solutions(problems) << "\n";

    return 0;
}
