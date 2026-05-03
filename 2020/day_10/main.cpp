#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <algorithm>

int find_chain(std::vector<int> &numbers) {
    auto [g1,g3,p] = std::accumulate(numbers.begin(), numbers.end(), std::tuple<int,int,int>{0, 0, 0}, 
        [](auto t, int v) { auto [g1, g3, p] = t; return std::tuple<int,int,int>{g1+(v==p+1 ? 1 : 0), g3+(v==p+3 ? 1 : 0), v}; });

    return g1*g3;
}

int64_t count_solutions(std::vector<int> &numbers) {
    std::vector<int64_t> solutions(numbers.size());
    solutions[0] = 1;

    for(int i=0; i<numbers.size(); i++) {
        int j=i+1;
        while(j<numbers.size() && numbers[j] <= numbers[i]+3) {
            solutions[j++] += solutions[i];
        }
    }

    return solutions.back();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input1.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<int> numbers;

    while(std::getline(input, line)) {
        numbers.push_back(std::stoi(line));
    }

    numbers.push_back(0);
    std::sort(numbers.begin(), numbers.end());
    numbers.push_back(numbers.back()+3);    

    std::cout << "Part1: " << find_chain(numbers) << "\n";
    std::cout << "Part2: " << count_solutions(numbers) << "\n";

    return 0;
}
