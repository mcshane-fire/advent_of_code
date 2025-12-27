#include <string>
#include <iostream>
#include <fstream>

int add_antinode(std::vector<std::string> &map, int x, int y) {
    if(y >= 0 && y < map.size() && x >= 0 && x < map[y].length()) {
        if(map[y][x] != '#') {
            map[y][x] = '#';
            return 1;
        }
        return 0;
    }
    return -1;    
}

int count_antinodes(std::vector<std::string> map, int limit) {
    int total = 0;

    std::unordered_map<char,std::vector<std::pair<int, int>>> nodes;

    for(int y=0; y<map.size(); y++) {
        for(int x=0; x<map[y].length(); x++) {
            char c = map[y][x];
            if(c != '.') {
                if(!nodes.contains(c)) {
                    nodes[c] = std::vector<std::pair<int, int>>();
                }
                nodes[c].push_back(std::pair<int,int>(x,y));
            }

        }
    }

    for(auto c : nodes) {
        for(int i=0; i<c.second.size()-1; i++) {
            for(int j=i+1; j<c.second.size(); j++) {
                auto &p1 = c.second[i];
                auto &p2 = c.second[j];
                int dx = p1.first - p2.first;
                int dy = p1.second - p2.second;

                if(limit) {
                    if(add_antinode(map, p1.first + dx, p1.second + dy) == 1) {
                        total++;
                    }
                    if(add_antinode(map, p2.first - dx, p2.second - dy) == 1) {
                        total++;
                    }
                }
                else {
                    int r = 0;
                    int k = 0;
                    while(r != -1) {
                        r = add_antinode(map, p1.first + (k*dx), p1.second + (k*dy));
                        if(r == 1) {
                            total++;
                        }
                        k++;
                    }
                    k = 0;
                    r = 0;
                    while(r != -1) {
                        r = add_antinode(map, p1.first - (k*dx), p1.second - (k*dy));
                        if(r == 1) {
                            total++;
                        }
                        k++;
                    }
                }
            }
        }
    }

    if(0) {
        std::cout << "\n";
        for(std::string line : map) {
            std::cout << line << "\n";
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part 1: " << count_antinodes(map, 1) << "\n";
    std::cout << "Part 1: " << count_antinodes(map, 0) << "\n";

    return 0;
}