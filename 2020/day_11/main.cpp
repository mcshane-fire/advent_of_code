#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>

int find_stable(std::vector<std::string> map, bool all_seeing) {
    int xd[] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yd[] = {0, -1, -1, -1, 0, 1, 1, 1};

    while(true) {
        std::vector<std::string> next;
        for(int y=0; y<map.size(); y++) {
            next.push_back("");
            for(int x=0; x<map[y].length(); x++) {
                if(map[y][x] == '.') {
                    next.back() += '.';
                } else {
                    int occupied = 0;
                    for(int d=0; d<8; d++) {
                        int ny = y;
                        int nx = x;
                        while(true) {
                            ny += yd[d];
                            nx += xd[d];
                            if(ny < 0 || ny >= map.size() || nx < 0 || nx >= map[ny].length()) {
                                break;
                            }

                            if(map[ny][nx] == '#') {
                                occupied++;
                                break;
                            }
                            if(!all_seeing || map[ny][nx] == 'L') {
                                break;
                            }
                        }
                    }
                    if(map[y][x] == 'L') {
                        next.back() += occupied == 0 ? '#' : 'L';
                    } else {
                        next.back() += occupied >= (all_seeing ? 5 : 4) ? 'L' : '#';
                    }
                }
            }
        }
        if(next == map) {
            break;
        }
        map.swap(next);
    }

    int occupied = 0;
    for(auto &s : map) {
        occupied += std::accumulate(s.begin(), s.end(), 0, [](int t, char c) { return t += (c == '#' ? 1 : 0); });
    }

    return occupied;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << find_stable(map, false) << "\n";
    std::cout << "Part2: " << find_stable(map, true) << "\n";

    return 0;
}
