#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int count_until_stopped(std::vector<std::string> &map) {
    int steps = 0;
    while(true) {
        bool moved = false;
        steps++;

        for(auto &v : map) {
            std::vector<std::pair<int,int>> moves;

            for(int i=0; i<v.size(); i++) {
                int np = i+1 == v.size() ? 0 : i+1;
                if(v[i] == '>' && v[np] == '.') {
                    moves.emplace_back(i, np);
                }
            }

            for(auto &p : moves) {
                std::swap(v[p.first], v[p.second]);
                moved = true;
            }
        }

        for(int x=0; x<map[0].length(); x++) {
            std::vector<std::pair<int,int>> moves;
            for(int y=0; y<map.size(); y++) {
                int np = y+1 == map.size() ? 0 : y+1;
                if(map[y][x] == 'v' && map[np][x] == '.') {
                    moves.emplace_back(y, np);
                }
            }

            for(auto &p : moves) {
                std::swap(map[p.first][x], map[p.second][x]);
                moved = true;
            }
        }

        if(!moved) {
            break;
        }
    }
    return steps;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1:" << count_until_stopped(map) << "\n";

    return 0;
}
