#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int find_product(std::vector<int> &numbers, int total) {

    for(auto n : numbers) {
        for(auto m : numbers) {
            if(n + m == total) {
                return n * m;
            }
        }
    }

    return 0;
}

int find_product_three(std::vector<int> &numbers, int total) {

    for(auto n : numbers) {
        for(auto m : numbers) {
            for(auto p : numbers) {
                if(n + m + p == total) {
                    return n * m *  p;
                }
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int> numbers;

    while(std::getline(input, line)) {
        numbers.push_back(std::stoi(line));
    }

    std::cout << "Part1:" << find_product(numbers, 2020) << "\n";
    std::cout << "Part2:" << find_product_three(numbers, 2020) << "\n";

    return 0;
}
