#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<int> cap;
    int cur = 0;

    while(std::getline(input, line)) {
        if(line.length() == 0) {
            cap.push_back(cur);
            cur = 0;
        }
        else {
            cur += std::stoi(line);
        }
    }
    cap.push_back(cur);

    std::nth_element(cap.begin(), cap.begin()+3, cap.end(), std::greater{}); 

    std::cout << "Part1: " << *std::max_element(cap.begin(), cap.begin()+3) << "\n";
    std::cout << "Part2: " << std::accumulate(cap.begin(), cap.begin()+3, 0) << "\n";


    return 0;
}