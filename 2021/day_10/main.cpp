#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

int64_t find_errors(std::vector<std::string> problems, bool incomplete) {
    int64_t ret = 0;
    std::map<char,char> pairs = {{'<', '>'}, {'(', ')'}, {'[', ']'}, {'{', '}'}};
    std::map<char,int> score = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    std::map<char,int64_t> complete = {{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};
    std::vector<int64_t> scores;

    for(auto &s : problems) {
        std::vector<char> expect;
        bool error = false;

        for(auto c : s) {
            if(pairs.contains(c)) {
                expect.push_back(pairs[c]);
            } else {
                if(expect.size() == 0 || expect.back() != c) {
                    ret += score[c];
                    error = true;
                    break;
                }
                expect.pop_back();
            }
        }

        if(incomplete && !error) {
            int64_t score = 0;
            while(expect.size() > 0) {
                char c = expect.back();
                expect.pop_back();
                score = (score * 5L) + complete[c];
            }
            scores.push_back(score);
        }
    }

    if(incomplete) {
        std::sort(scores.begin(), scores.end());
        ret = scores[(scores.size()-1)/2];
    }

    return ret;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> problems;

    while(std::getline(input, line)) {
        problems.push_back(line);
    }

    std::cout << "Part1: " << find_errors(problems, false) << "\n";
    std::cout << "Part2: " << find_errors(problems, true) << "\n";

    return 0;
}
