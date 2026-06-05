#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <numeric>

int reverse(int num, int width) {
    int ret = 0;
    for(int i=0; i<width; i++) {
        ret |= (1 & (num >> i)) << (width-i-1);
    }
    return ret;
}

int bitselect(std::vector<int> vec, int pos) {
    int ret = 0;
    for(int i=0; i<vec.size(); i++) {
        ret |= ((vec[i] >> pos) & 1) << i;
    }
    return ret;
}

int bitcount(int num) {
    int ret = 0;
    while(num) {
        ret++;
        num &= num-1;
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
    std::vector<int> middle[8];
    int bits;

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

        if(lines > 1 && lines < line.length()) {
            int m = 0;
            for(int i=1; i<line.length()-1; i++) {
                m |= (line[i] == '#' ? 1 : 0) << (i-1);
            }
            middle[0].push_back(m);
            bits += bitcount(m);
        }

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
            for(int m : middle[0]) {
                middle[1].push_back(reverse(m, lines-2));
            }

            // vflip 0-1 into 2-3
            for(int i=0; i<2; i++) {
                border[TOP][i+2] = border[BOTTOM][i];
                border[BOTTOM][i+2] = border[TOP][i];
                border[LEFT][i+2] = reverse(border[LEFT][i], lines);
                border[RIGHT][i+2] = reverse(border[RIGHT][i], lines);
                for(auto it = middle[i].rbegin(); it != middle[i].rend(); it++) {
                    middle[i+2].push_back(*it);
                }
            }

            // transpose 0-3 into 4-7
            for(int i=0; i<4; i++) {
                border[TOP][i+4] = border[LEFT][i];
                border[LEFT][i+4] = border[TOP][i];
                border[BOTTOM][i+4] = border[RIGHT][i];
                border[RIGHT][i+4] = border[BOTTOM][i];
                for(int j=0; j<lines-2; j++) {
                    middle[i+4].push_back(bitselect(middle[i], j));
                }
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

int64_t find_layout(std::vector<Tile> tiles) {
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

bool match(std::map<Pos,Tile> &map, int xt, int x, int yt, int y, int num) {
    auto p = Pos(xt, yt);
    int got = map[p].lines-2-x;
    int val = map[p].middle[map[p].orientation][y] >> x;
    while(got < 20) {
        p = p + Pos(1,0);
        val |= map[p].middle[map[p].orientation][y] << got;
        got += map[p].lines-2;
    }

    return (val & num) == num;
}

int spot_sea_monsters(std::map<Pos,Tile> &map) {
    //          |        #    
    //#    ##   | ##    ###  
    // #  #  #  |#  #  #    
    //0123456789|0123456789

    auto it = map.begin();
    auto eit = map.rbegin();
    Pos c0 = (*it).first;
    Pos c2 = (*eit).first;
    int lines = (*it).second.lines-2;
    int found = 0;

    for(int x=0; x<(c2.x - c0.x + 1) * lines - 19; x++) {
        for(int y=0; y<(c2.y - c0.y + 1) * lines - 2; y++) {
            if(match(map, c0.x + (x / lines), x % lines, c0.y + (y / lines), y % lines, 262144) &&
               match(map, c0.x + (x / lines), x % lines, c0.y + ((y+1) / lines), (y+1) % lines, 923745) &&
               match(map, c0.x + (x / lines), x % lines, c0.y + ((y+2) / lines), (y+2) % lines, 74898)) {
                found++;
            }
        }
    }

    if(false && found > 0) {
        for(Pos p = c0; p.y <= c2.y; p = p + Pos(0,1)) {
            for(int iy = 0; iy < lines; iy++) {
                for(Pos p2 = p; p2.x <= c2.x; p2 = p2 + Pos(1,0)) {
                    for(int i=0; i<lines; i++) {
                        std::cout << (((map[p2].middle[map[p2].orientation][iy] >> i) & 1) == 1 ? "#" : ".");
                    }
                }
                std::cout << "\n";
            }
        }
    }

    return found;
}

int count_sea_monsters(std::vector<Tile> &tiles) {
    int ret = 0;
    for(int i=0; i<8; i++) {
        for(auto &t : tiles) {
            t.orientation = -1;
        }

        std::map<Pos,Tile> map;
        tiles[0].orientation = i;
        map[Pos(0,0)] = tiles[0];

        try_layout(tiles, map);
        if(map.size() == tiles.size()) {
            ret = std::max(ret, spot_sea_monsters(map));
        }
    }

    return std::accumulate(tiles.begin(), tiles.end(), 0, [](int s, const Tile &t) { return s + t.bits; }) - (ret * 15);
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
    std::cout << "Part1: " << count_sea_monsters(tiles) << "\n";

    return 0;
}
