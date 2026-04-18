#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <climits>
#include <algorithm>
#include <numeric>

struct Pair {
    char a;
    char b;

    bool operator==(const Pair &p) const {
        return a == p.a && b == p.b;
    }

    bool operator<(const Pair &p) const {
        return a == p.a ? (b < p.b) : (a < p.a);
    }
};

int64_t count_difference(std::map<Pair,char> &rules, std::map<Pair,int64_t> sequence, int iterations, char start) {
    for(int i=0; i<iterations; i++) {
        std::map<Pair,int64_t> next_sequence;

        for(auto &n : sequence) {
            auto &r = rules[n.first];
            next_sequence[{n.first.a,r}] += n.second;
            next_sequence[{r,n.first.b}] += n.second;
        }

        sequence.clear();
        next_sequence.swap(sequence);
    }

    std::map<char,int64_t> freq = {{start, 1L}};
    for(auto &n : sequence) {
        freq[n.first.b] += n.second;
    }

    return (*std::max_element(freq.begin(), freq.end(), [](const auto &a, const auto &b) { return a.second < b.second; })).second -
        (*std::min_element(freq.begin(), freq.end(), [](const auto &a, const auto &b) { return a.second < b.second; })).second;
        
        //accumulate(sequence.begin(), sequence.end(), 0, [](int t, const auto &p) { return t + p.second; });
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::map<Pair,char> rules;
    std::map<Pair,int64_t> sequence;
    char start;

    std::getline(input, line);
    start = line[0];
    for(auto it = line.begin(); it != std::prev(line.end()); it++) {
        sequence[{*it, *(std::next(it))}] += 1;
    }

    while(std::getline(input, line)) {
        if(line.length() > 0) {
            auto it = line.begin();
            rules[{*it, *(std::next(it))}] = *(line.rbegin());
        }
    }

    std::cout << "Part1: " << count_difference(rules, sequence, 10, start) << "\n";
    std::cout << "Part2: " << count_difference(rules, sequence, 40, start) << "\n";

    return 0;
}
