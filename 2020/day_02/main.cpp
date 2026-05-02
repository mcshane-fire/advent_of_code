#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

bool check_password_count(char letter, int min_count, int max_count, std::string password) {
    int num = 0;
    for(auto c : password) {
        if(c == letter) {
            num++;
        }
    }

    return num >= min_count && num <= max_count;
}

bool check_password_position(char letter, int min_offset, int max_offset, std::string password) {
    return password[min_offset-1] == letter ^ password[max_offset-1] == letter;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int> numbers;
    std::regex pat = std::regex("^(\\d+)-(\\d+) (.): (.+)$");
    std::smatch res;

    int num_1 = 0;
    int num_2 = 0;
    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            std::string letter = res[3];
            if(check_password_count(letter[0], std::stoi(res[1]), std::stoi(res[2]), std::string(res[4]))) {
                num_1++;
            }
            if(check_password_position(letter[0], std::stoi(res[1]), std::stoi(res[2]), std::string(res[4]))) {
                num_2++;
            }            
        }
    }

    std::cout << "Part1:" << num_1 << "\n";
    std::cout << "Part2:" << num_2 << "\n";

    return 0;
}
