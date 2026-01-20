#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>

int predict_next(std::vector<std::vector<int>> sequences, bool prev = false) {
    int total = 0;
    int prev_total = 0;

    for(auto sequence : sequences) {
        std::vector<std::vector<int>> diffs;
        diffs.emplace_back(sequence);

        while(true) {
            std::vector<int> next(diffs.back().size());
            std::adjacent_difference(diffs.back().begin(), diffs.back().end(), next.begin());
            if(*std::max_element(next.begin()+diffs.size(), next.end()) == 0 && *std::min_element(next.begin()+diffs.size(), next.end()) == 0) {
                break;
            }
            diffs.emplace_back(next); 
        }

        int inc = 0;
        for(auto it = diffs.rbegin(); it != diffs.rend(); it++) {
            inc += (*it).back();
        }
        total += inc;

        inc = 0;
        for(int i=diffs.size()-1; i>=0; i--) {
            inc = diffs[i][i] - inc;
        }
        prev_total += inc;
    }

    return prev ? prev_total : total;
}


int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::vector<int>> sequences;

    while(std::getline(input, line)) {
        std::stringstream ss(line);
        std::string word;
        std::vector<int> sequence;
        while(getline(ss, word, ' ')) {
            sequence.push_back(std::stoi(word));
        }
        sequences.emplace_back(sequence);
    }

    std::cout << "Part1: " << predict_next(sequences) << "\n";
    std::cout << "Part2: " << predict_next(sequences, true) << "\n";
    
    return 0;
}