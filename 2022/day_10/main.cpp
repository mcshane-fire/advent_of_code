#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

int add(int cycle, int x, std::vector<std::string>& crt) {
    crt[(cycle-1)/40].append(std::abs(x +1 - ((cycle)%40)) < 2 ? "#" : ".");

    if(cycle % 20 == 0 && ((cycle/20) & 1)) {
        return x * cycle;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    auto crt = std::vector<std::string>(7);

    int cycle = 0;
    int x = 1;
    int total = 0;
    while(cycle < 240) {
        if(!std::getline(input, line)) {
            input.clear();
            input.seekg(0, std::ios::beg);
            continue;
        }
     
        total += add(++cycle, x, crt);
        
        if(line != "noop") { 
            total += add(++cycle, x, crt);
            x += std::stoi(line.substr(5));
        }
    }    

    std::cout << "Part1: " << total << "\n";

    std::cout << "Part2:\n";
    for(auto s : crt) {
        std::cout << s << "\n";
    }

    return 0;
}