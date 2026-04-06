#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>

int score_winner(std::vector<int> &numbers, std::vector<std::vector<std::vector<int>>> boards, int ith)
{
    std::set<int> called;
    int winners = 0;
    for(auto n : numbers) {
        called.insert(n);
        for(auto &board : boards) {
            for(auto &row : board) {
                if(std::find_if(row.begin(), row.end(), [called](int a){ return !called.contains(a); }) == row.end()) {
                    if(++winners == ith) {
                        int total = std::accumulate(board.back().begin(), board.back().end(), 0, [called](int t, int a) {
                            return t + (called.contains(a) ? 0 : a); });
                        return total * n;
                    }
                    board.resize(0);
                    break;  
                }
            }
        }
    }

    return 0;
}


void decode(std::string line, std::vector<int> &vec) {
    while(line.length() > 0) {
        size_t pos;
        vec.push_back(std::stoi(line, &pos));
        if(pos >= line.length()) {
            break;
        }
        line = line.substr(pos+1);
    }
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int> numbers;
    std::vector<std::vector<std::vector<int>>> boards;

    std::getline(input, line);
    decode(line, numbers);

    while(std::getline(input, line)) {
        if(line.length() == 0) {
            boards.push_back(std::vector<std::vector<int>>());
        } else {
            boards.back().push_back(std::vector<int>());
            decode(line, boards.back().back());
        }
    }

    for(auto &b : boards) {
        std::vector<int> all;
        for(int i=0; i<b[0].size(); i++) {
            b.push_back(std::vector<int>());
            for(int j=0; j<b[0].size(); j++) {
                b.back().push_back(b[j][i]);
                all.push_back(b[j][i]);
            }
        }
        b.push_back(all);
    }

    std::cout << "Part1: " << score_winner(numbers, boards, 1) << "\n";
    std::cout << "Part2: " << score_winner(numbers, boards, boards.size()) << "\n";

    return 0;
}
