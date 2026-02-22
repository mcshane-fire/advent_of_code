#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

void find_different(std::string line, int len) {
    auto bit = line.begin();
    auto eit = bit+len;
    std::set<char> s;
    do {
        s.erase(s.begin(), s.end());
        s.insert(bit++, eit++);
    } while(bit != line.end() && s.size() != len);

    std::cout << "Part(" << len << "): " << (eit-line.begin()-1) << "\n";
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    while(std::getline(input, line)) {
        find_different(line, 4);
        find_different(line, 14);
    }

    return 0;
}