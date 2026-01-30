#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>

int count_destinations(std::vector<std::string>& map, int steps) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::set<std::pair<int,int>> frontier;
    int x, y;
    for(y=0; y<map.size(); y++) {
        if((x = map[y].find('S')) != std::string::npos) {
            break;
        }
    } 
    frontier.insert({x, y});
    for(int i=0; i<steps; i++) {
        std::set<std::pair<int,int>> next_frontier;

        for(auto& p : frontier) {
            for(int dir=0; dir<4; dir++) {
                int ny = p.second+yd[dir];
                int nx = p.first+xd[dir];
                if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length()) {
                    if(map[ny][nx] != '#') {
                        next_frontier.insert({nx, ny});
                    }
                }
            }
        }
        frontier = next_frontier;
    }

    return frontier.size();
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_destinations(map, 64) << "\n";

    return 0;
}


