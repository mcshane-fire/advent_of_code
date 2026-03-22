#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

int64_t snafu_to_int64(std::string snafu) {
    int64_t total = 0;
    int64_t col = 1;
    std::map<char,int64_t> conv = {{'2',2}, {'1',1}, {'0',0}, {'-',-1}, {'=',-2}};
    for(auto it = snafu.rbegin(); it != snafu.rend(); it++) {
        total += col * conv[*it];
        col *= 5;
    }

    return total;
}

std::string int64_to_snafu(int64_t num) {
    std::string ret = "";
    while(num > 0) {
        int d = num % 5;
        if(d <= 2) {
            ret += '0' + d;
        } else if(d == 3) {
            ret += '=';
            num += 2;
        } else if(d == 4) {
            ret += '-';
            num += 1;
        }
        num /= 5;
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    int64_t total = 0;

    while(std::getline(input, line)) {
        total += snafu_to_int64(line);
    }
    std::cout << "Part1: " << int64_to_snafu(total) << "\n";

    return 0;
}
