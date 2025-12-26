#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <set>

enum SearchMode {
    COUNT_STEPS,
    COUNT_LOOPS,
    FIND_LOOP,
};

int check_route(std::vector<std::string> map, int start_x, int start_y, int start_dir, SearchMode mode) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};
    int x = start_x;
    int y = start_y;
    int dir = start_dir;
    int total = mode == COUNT_STEPS ? 1 : 0;
    std::vector<std::vector<std::pair<int, int>>> turns;

    //std::cout << "check_route(" << x << ", "<< y << ", " <<dir<< " mode "<<mode<<")\n";

    if(mode == FIND_LOOP) {
        for(int i=0; i<map.size(); i++) {
            std::vector<std::pair<int, int>> rl;
            turns.push_back(rl);
        }
    }

    while(1) {
        int nx = x + xd[dir];
        int ny = y + yd[dir];
        if(nx < 0 || nx >= map[ny].length() || ny < 0 || ny >= map.size()) {
            break;
        }
        char c = map[ny][nx];
        if(c == '#' || c == 'O') {
            if(mode == FIND_LOOP) {
                for(auto pp : turns[y]) {
                    if(pp.first == x && pp.second == dir) {
                        //std::cout << "Found loop!\n";
                        //for(auto s : map) {
                        //    std::cout << s << "\n";
                        //}
                        return -1;
                    }
                }
                std::pair<int, int> tt(x, dir);
                turns[y].push_back(tt);
            }
            dir--;
            if(dir < 0) {
                dir = 3;
            }
            continue;
        } else if (c == '.') {
            if(mode == COUNT_STEPS) {
                map[ny][nx] = 'X';
                total++;
            } else if(mode == COUNT_LOOPS) {
                map[ny][nx] = 'O';
                if(check_route(map, start_x, start_y, start_dir, FIND_LOOP) == -1) {
                    total++;
                }
                map[ny][nx] = '+';
            }
        }
        x = nx;
        y = ny;
    }

    if(0 && mode == COUNT_STEPS) {
        for(auto s : map) {
            std::cout << s << "\n";
        }
    }

    return total;
}

int count_visited_tiles(std::vector<std::string> map, SearchMode mode) {
    int x, y;

    for(y = 0; y<map.size(); y++) {
        std::string line = map[y];
        x = line.find('^');
        if(x != line.npos) {
            break;
        }
    }
    //std::cout << "Found " << x << ", " << y << "\n";
    return check_route(map, x, y, 1, mode);
}

int main(int argc, char *argv[]) {
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::string> lines;

    while(std::getline(input, line)) {
        lines.push_back(line);
    }

    std::cout << "Part 1: " << count_visited_tiles(lines, COUNT_STEPS) << "\n";
    std::cout << "Part 2: " << count_visited_tiles(lines, COUNT_LOOPS) << "\n";

    return 0;
}