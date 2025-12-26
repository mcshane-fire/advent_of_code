#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

int find_solution(uint64_t target, std::vector<uint64_t>::iterator start, std::vector<uint64_t>::iterator end, int enable_concat) {
    // handle only a single number in the list
    if(start == end-1) {
        //std::cout << target << " Final element: " << *start << "\n";
        return target == *start;
    }

    --end;

    if(target % *end == 0) {
        //std::cout << target << " Trying division by " << *end << "\n";
        if(find_solution(target / *end, start, end, enable_concat) == 1) {
            return 1;
        }
    }

    if(enable_concat) {
        std::string ts = std::to_string(target);
        std::string es = std::to_string(*end);
        if(ts.ends_with(es) && es.length() < ts.length()) {
            //std::cout << target << " Trying concat, " << *end << " -> " << ts.substr(0, ts.length() - es.length()) << "\n";
            if(find_solution(std::stoull(ts.substr(0, ts.length() - es.length())), start, end, enable_concat) == 1) {
                return 1;
            }
        }
    }

    //std::cout << target << " Trying substraction by " << *end << "\n";
    return find_solution(target - *end, start, end, enable_concat);
}

uint64_t find_solutions(std::vector<std::vector<uint64_t>> problems, int enable_concat) {
    uint64_t total = 0;

    for(auto p : problems) {
        if(find_solution(p[0], p.begin()+1, p.end(), enable_concat)) {
            //std::cout << "Solved " << p[0] << "\n";
            total += p[0];
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::vector<uint64_t>> problems;
    std::regex num("\\d+");
    std::smatch res;

    while(std::getline(input, line)) {
        std::string::const_iterator start(line.cbegin());
        std::vector<uint64_t> all;

        while(std::regex_search(start, line.cend(), res, num)) {
            all.push_back(std::stoull(res[0]));
            start = res.suffix().first;
        }
        problems.push_back(all);
    }

    std::cout << "Part 1: " << find_solutions(problems, 0) << "\n";
    std::cout << "Part 2: " << find_solutions(problems, 1) << "\n";

    return 0;
}