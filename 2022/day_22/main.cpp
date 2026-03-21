#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

struct Pos {
    int x = 0;
    int y = 0;

    Pos operator+(const Pos &p) {
        return {x+p.x, y+p.y};
    }

    bool operator==(const Pos &p) {
        return x == p.x && y == p.y;
    }

    bool operator<(const Pos &p) const {
        return x == p.x ? (y < p.y) : (x < p.x);
    }
};

struct Wrap {
    std::vector<Pos>::iterator fit;
    std::vector<Pos>::reverse_iterator rit;
    int dir;
    bool fwd;

    Wrap(std::vector<Pos>::iterator _fit, int _dir) : fit(_fit), dir(_dir), fwd(true) {};
    Wrap(std::vector<Pos>::reverse_iterator _rit, int _dir) : rit(_rit), dir(_dir), fwd(false) {};

    Pos next(Pos p, int inc) {
        if(fwd) {
            return *(fit + inc);
        } else {
           return *(rit + inc);
        }
    }
};

int trace_path(std::vector<std::string> &map, std::string instructions, bool cube = false) {
    Pos p = {(int) (std::find(map[0].begin(), map[0].end(), '.') - map[0].begin()), 0};
    std::vector<Pos> pd = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    int dir = 0;
    bool number = true;

    std::vector<Pos> top;
    std::vector<Pos> bottom;
    std::vector<Pos> left;
    std::vector<Pos> right;

    int size = map.size();

    for(int y=0; y<map.size(); y++) {
        left.push_back(Pos((int) (std::find_if(map[y].begin(), map[y].end(), [](char c) {return c != ' '; }) - map[y].begin()), y));
        right.push_back(Pos(map[y].length() - 1 - (int) (std::find_if(map[y].rbegin(), map[y].rend(), [](char c) {return c != ' '; }) - map[y].rbegin()), y));

        size = std::min(size, right.back().x - left.back().x + 1);

        for(int x=top.size(); x<map[y].length(); x++) {
            top.push_back(Pos(x, map.size()));
            bottom.push_back(Pos(x, 0));
        }

        for(int x=0; x<map[y].length(); x++) {
            if(map[y][x] != ' ') {
                top[x] = std::min(top[x], Pos(x,y));
                bottom[x] = std::max(bottom[x], Pos(x,y));
            }
        }
    }

    std::vector<std::string> map2 = map;
    std::vector<char> symbol = {'>', 'v', '<', '^'};

    // data for test_input.txt
    std::vector<std::vector<Wrap>> wrap2 = {
        {{right.rbegin(), 2}, {top.rbegin(),1}, {right.rend()-size,2}},
        {{bottom.rbegin()+size,3}, {left.rbegin(),0}, {bottom.rend()-size,3}, {left.rbegin()+size,0}},
        {{top.begin()+size,1}, {bottom.rbegin(),3}, {bottom.rbegin()+(2*size),3}},
        {{top.rbegin()+size,1}, {left.begin(), 0}, {top.rend()-size,1}, {right.rbegin()+size,2}}
    };

    // data for input.txt
    std::vector<std::vector<Wrap>> wrap = {
        {{right.rbegin()+size,2}, {bottom.end()-size,3}, {right.rend()-size,2}, {bottom.begin()+size,3}},
        {{top.end()-size,1}, {right.end()-size,2}, {right.begin()+size,2}},
        {{left.rbegin()+size,0}, {top.begin(),1}, {left.rend()-size,0}, {top.begin()+size,1}},
        {{left.begin()+size,0}, {left.end()-size,0}, {bottom.begin(),3}}
    };

    while(instructions.length() > 0) {
        if(number) {
            size_t read = 0;
            int n = std::stoi(instructions, &read);
            instructions = instructions.substr(read);
            number = false;

            for(int i=0; i<n; i++) {
                Pos np;
                int nd = dir;
                if(!cube) {
                    switch(dir) {
                        case 0: np = p == right[p.y] ? left[p.y] : p + pd[dir]; break;
                        case 1: np = p == bottom[p.x] ? top[p.x] : p + pd[dir]; break;
                        case 2: np = p == left[p.y] ? right[p.y] : p + pd[dir]; break;
                        case 3: np = p == top[p.x] ? bottom[p.x] : p + pd[dir]; break;
                    }
                } else {
                    np = p + pd[dir];
                    switch(dir) {
                        case 0: 
                            if(p == right[p.y]) {
                               np = wrap[0][p.y/size].next(p, p.y%size);
                               nd = wrap[0][p.y/size].dir;
                            }
                            break;
                        case 1:
                            if(p == bottom[p.x]) {
                                np = wrap[1][p.x/size].next(p, p.x%size);
                                nd = wrap[1][p.x/size].dir;
                            }
                            break;
                        case 2:
                            if(p == left[p.y]) {
                                np = wrap[2][p.y/size].next(p, p.y%size);
                                nd = wrap[2][p.y/size].dir;
                            }
                            break;
                        case 3:
                            if(p == top[p.x]) {
                                np = wrap[3][p.x/size].next(p, p.x%size);
                                nd = wrap[3][p.x/size].dir;
                            }
                            break;
                    }
                }

                if(map[np.y][np.x] == '#') {
                    break;
                }

                map2[p.y][p.x] = symbol[dir];
                map2[np.y][np.x] = symbol[dir];

                p = np;
                dir = nd;
            }

        } else {
            if(instructions[0] == 'R') {
                dir = (dir+1) & 3;
            } else {
                dir = (dir+3) & 3;
            }
            instructions = instructions.substr(1);
            number = true;
        }

    }

    return (1000 * (p.y+1)) + (4 * (p.x+1)) + dir;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;
    std::string instructions;
    bool in_map = true;

    while(std::getline(input, line)) {
        if(in_map) {
            if(line.length() == 0) {
                in_map = false;
            } else {
                map.push_back(line);
            }
        } else {
            instructions = line;
        }
    }

    std::cout << "Part1: " << trace_path(map, instructions) << "\n";
    std::cout << "Part2: " << trace_path(map, instructions, true) << "\n";

    return 0;
}
