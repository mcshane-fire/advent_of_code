#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>

int reverse(int num, int width) {
    int ret = 0;
    for(int i=0; i<width; i++) {
        ret |= (1 & (num >> i)) << (width-i-1);
    }
    return ret;
}

enum Border {
    TOP,
    LEFT,
    BOTTOM,
    RIGHT,
};

struct Tile {
    int64_t num;
    int lines;
    int border[4][8];
    int orientation;

    Tile() : lines(0), num(0LL), orientation(-1) {}

    Tile(std::string line) {
        num = std::stoll(line.substr(5));
        lines = 0;
        orientation = -1;
    }

    void add(std::string line) {
        if(lines == 0) {
            border[LEFT][0] = 0;
            border[RIGHT][0] = 0;
            border[TOP][0] = 0;
            for(int i=0; i<line.length(); i++) {
                border[TOP][0] |= (line[i] == '#' ? 1 : 0) << i;
            }
        }

        border[LEFT][0] |= (line[0] == '#' ? 1 : 0) << lines;
        border[RIGHT][0] |= (line[line.length()-1] == '#' ? 1 : 0) << lines;
        lines++;

        if(lines == line.length()) {
            border[BOTTOM][0] = 0;
            for(int i=0; i<line.length(); i++) {
                border[BOTTOM][0] |= (line[i] == '#' ? 1 : 0) << i;
            }

            // hflip 0 into 1
            border[RIGHT][1] = border[LEFT][0];
            border[LEFT][1] = border[RIGHT][0];
            border[TOP][1] = reverse(border[TOP][0], lines);
            border[BOTTOM][1] = reverse(border[BOTTOM][0], lines);

            // vflip 0-1 into 2-3
            for(int i=0; i<2; i++) {
                border[TOP][i+2] = border[BOTTOM][i];
                border[BOTTOM][i+2] = border[TOP][i];
                border[LEFT][i+2] = reverse(border[LEFT][i], lines);
                border[RIGHT][i+2] = reverse(border[RIGHT][i], lines);
            }

            // transpose 0-3 into 4-7
            for(int i=0; i<4; i++) {
                border[TOP][i+4] = border[LEFT][i];
                border[LEFT][i+4] = border[TOP][i];
                border[BOTTOM][i+4] = border[RIGHT][i];
                border[RIGHT][i+4] = border[BOTTOM][i];
            }
        }
    }
};

struct Pos {
    int x = 0;
    int y = 0;

    Pos operator+(const Pos &p) const {
        return {x+p.x, y+p.y};
    }

    auto operator<=>(const Pos &p) const = default;

    friend std::ostream& operator<<(std::ostream &os, const Pos &p) {
        os << p.x << "," << p.y;
        return os;
    }
};

struct Move {
    Pos direction;
    Border src;
    Border dest;
};

void try_layout(std::vector<Tile> &tiles, std::map<Pos,Tile> &map) {
    std::vector<Move> moves = {
        {{1,0}, RIGHT, LEFT},
        {{-1,0}, LEFT, RIGHT},
        {{0,1}, BOTTOM, TOP},
        {{0,-1}, TOP, BOTTOM}};

    while(true) {
        std::map<Pos,std::set<std::pair<int,int>>> candidates;

        for(auto &v : map) {
            //std::cout << "looking for match from " << v.second.num << " orient:" << v.second.orientation << "\n";
            for(auto &pd : moves) {
                Pos cp = v.first + pd.direction;
                if(!map.contains(cp)) {
                    //std::cout << " space in " << cp << "\n";
                    for(int i=0; i<tiles.size(); i++) {
                        if(tiles[i].orientation == -1) {
                            for(int orient=0; orient<8; orient++) {
                                if(v.second.border[pd.src][v.second.orientation] == tiles[i].border[pd.dest][orient]) {
                                    // check other directions
                                    bool collision = false;
                                    for(auto &tpd : moves) {
                                        Pos check = cp + tpd.direction;
                                        if(map.contains(check) && tiles[i].border[tpd.src][orient] != map[check].border[tpd.dest][map[check].orientation]) {
                                            collision = true;
                                        } 
                                    }
                                    if(!collision) {
                                        //std::cout << "  potential border from " << tiles[i].num << " orient:" << orient << "\n"; 
                                        candidates[cp].insert({i, orient});
                                    } else {
                                        std::cout << "  collision failed\n";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        bool placed = false;
        for(auto &c : candidates) {
            if(c.second.size() == 1) {
                auto p = *c.second.begin();
                //std::cout << "Placing candidate " << tiles[p.first].num << "\n";
                tiles[p.first].orientation = p.second;
                map[c.first] = tiles[p.first];
                placed = true;
            }
        }

        if(!placed) {
            break;
        }
    }
}

int64_t find_layout(std::vector<Tile> &tiles) {
    // assume we can start from any tile
    std::map<Pos,Tile> map;
    tiles[0].orientation = 0;
    map[Pos(0,0)] = tiles[0];

    try_layout(tiles, map);

    if(map.size() == tiles.size()) {
        auto it = map.begin();
        auto eit = map.rbegin();
        Pos c0 = (*it).first;
        Pos c2 = (*eit).first;
        Pos c1 = Pos(c0.x, c2.y);
        Pos c3 = Pos(c2.x, c0.y);    
        return  map[c0].num * map[c1].num * map[c2].num * map[c3].num;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Tile> tiles;

    while(std::getline(input, line)) {
        if(line[0] == 'T') {
            tiles.emplace_back(line);
        } else if(line.length() > 0) {
            tiles.back().add(line);
        }
    }

    std::cout << "Part1: " << find_layout(tiles) << "\n";

    return 0;
}
