#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

int64_t count_trees(std::vector<std::string> &map, int yd, int xd) {
    int64_t num = 0;
    int x = 0;
    int y = 0;
    while(y < map.size()) {
        if(map[y][x % map[y].length()] == '#') {
            num++;
        }
        y += yd;
        x += xd;
    }

    return num;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_trees(map, 1, 3) << "\n";
    std::cout << "Part2: " << count_trees(map, 1, 1) * count_trees(map, 1, 3) * count_trees(map, 1, 5) * count_trees(map, 1, 7) * count_trees(map, 2, 1) << "\n";

    return 0;
}