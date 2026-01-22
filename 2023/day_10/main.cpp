#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct pos {
    int x, y, d;
};

int navigate(std::vector<std::string>& map, std::vector<struct pos>& edge, std::vector<struct pos>& back, std::vector<std::vector<int>>& walls) {
    std::vector<std::map<char,int>> next = {{},
                                            {{'-',1}, {'7',4}, {'J',2}, {'S',1}},
                                            {{'|',2}, {'7',3}, {'F',1}, {'S',2}},
                                            {{'-',3}, {'L',2}, {'F',4}, {'S',3}},
                                            {{'|',4}, {'L',1}, {'J',3}, {'S',4}}};

    int yd[] = {0, 0, -1, 0, 1};
    int xd[] = {0, 1, 0, -1, 0};
    int steps = 0;
    int result = 0;

    do {
        std::vector<struct pos> next_edge = {};

        steps++;
        for(auto p : edge) {
            int nx = p.x + xd[p.d];
            int ny = p.y + yd[p.d];
            if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length()) {
                int nd = next[p.d][map[ny][nx]];
                if(nd > 0) {
                    for(auto np : next_edge) {
                        if(np.x == nx && np.y == ny) {
                            back.push_back(np);
                            back.emplace_back(nx, ny, nd);
                            result = steps;
                        }
                    }
                    next_edge.emplace_back(nx, ny, nd);

                    if(walls.size() > 0 && result == 0 && map[ny][nx] != '-') {
                        walls[ny].push_back(nx);
                    }
                }
            }
        }

        edge = next_edge;
    } while(edge.size() > 0 && result == 0);

    return result;
}

std::pair<int,int> find_loop(std::vector<std::string>& map) {
    int x, y;
    for(y=0; y<map.size(); y++) {
        if((x = map[y].find('S')) != std::string::npos) {
            break;
        }
    }

    std::vector<struct pos> edge = {{x, y, 1}, {x, y, 2}, {x, y, 3}, {x, y, 4}};
    std::vector<struct pos> back = {};
    std::vector<std::vector<int>> walls = {};

    int result = navigate(map, edge, back, walls);

    walls = std::vector<std::vector<int>>(map.size());
    if(map[back[0].y][back[0].x] != '-') {
        walls[back[0].y].push_back(back[0].x);
    }

    navigate(map, back, edge, walls);

    int fd = (std::min(back[0].d,back[1].d) * 10) + std::max(back[0].d,back[1].d);
    std::map<int,char> start_char = {{13,'-'}, {24,'|'}, {23,'F'}, {12,'7'}, {34,'L'}, {14,'J'}};
    map[y][x] = start_char[fd];

    int area = 0;
    for(int i=0; i<walls.size(); i++) {
        std::sort(walls[i].begin(), walls[i].end());
        bool inside = false;
        int j=0;
        while(j < walls[i].size()) {
            char c = map[i][walls[i][j]];

            if(c == '|') {
                inside = !inside;
            } else {
                char cn = map[i][walls[i][j+1]];
                if((c == 'F' && cn == 'J') || (c == '7' && cn == 'L') || (c == 'J' && cn == 'F') || (c == 'L' && cn == '7')) {
                    inside = !inside;
                }
                j += 1;
            }
            if(inside) {
                area += walls[i][j+1] - walls[i][j] - 1;
            }
            j++;
        }
    }

    return {result, area};
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::pair<int,int> res = find_loop(map);

    std::cout << "Part1: " << res.first << "\n";
    std::cout << "Part2: " << res.second << "\n";
    
    return 0;
}