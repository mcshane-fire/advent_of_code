#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <regex>
#include <ranges>
#include <numeric>
#include <set>

void add_views(std::set<int> &seen, std::vector<int> &row, int con, int mul, bool reverse) {
    int max = -1;
    for(int i=0; i<row.size(); i++) {
        int ri = reverse ? row.size()-i-1 : i;
        if(row[ri] > max) {
            seen.insert(con + (mul * ri));
            max = row[ri];
        }
    }
}

int score_view(std::vector<int> &row, int pos) {
    int vl = pos;
    int vr = pos;

    while(vl > 0 && row[--vl] < row[pos]) {}
    while(vr < row.size()-1 && row[++vr] < row[pos]) {}

    return (pos-vl) * (vr-pos);
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::getline(input, line);
    std::set<int> seen;

    auto rows = std::vector<std::vector<int>>(line.length());
    auto cols = std::vector<std::vector<int>>(line.length());
    auto rit = rows.begin();

    do {
        auto cit = cols.begin();
        for(auto c : line) {
            (*rit).push_back(c - '0');
            (*cit++).push_back(c - '0');
        }
        rit++;
    }
    while(std::getline(input, line));

    for(int i=0; i<rows.size(); i++) {
        add_views(seen, rows[i], i*rows.size(), 1, false);
        add_views(seen, rows[i], i*rows.size(), 1, true);
        add_views(seen, cols[i], i, rows.size(), false);
        add_views(seen, cols[i], i, rows.size(), true);
    }

    std::cout << "Part1: " << seen.size() << "\n";

    int max = 0;
    for(int i=1; i<rows.size()-1; i++) {
        for(int j=1; j<cols.size()-1; j++) {
            max = std::max(max, score_view(rows[i], j) * score_view(cols[j], i));
        }
    }

    std::cout << "Part2: " << max << "\n";

    return 0;
}