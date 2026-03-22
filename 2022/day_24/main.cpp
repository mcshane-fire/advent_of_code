#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <set>
#include <map>

struct Position {
    int x = 0;
    int y = 0;

    Position operator+(const Position &p) const {
        return {x+p.x, y+p.y};
    }

    bool operator==(const Position &p) const {
        return x == p.x && y == p.y;
    }

    bool operator<(const Position &p) const {
        return x == p.x ? (y < p.y) : (x < p.x);
    }
};

struct Blizzards {
    std::vector<std::vector<bool>> left;
    std::vector<std::vector<bool>> right;
    std::vector<std::vector<bool>> up;
    std::vector<std::vector<bool>> down;
    int width;
    int height;

    Blizzards(int w, int h) : width(w), height(h),
            left(std::vector<std::vector<bool>>(h)),
            right(std::vector<std::vector<bool>>(h)),
            up(std::vector<std::vector<bool>>(w)),
            down(std::vector<std::vector<bool>>(w)) {};

};

int mod(const int p, const int size) {
    return p % size < 0 ? ((p % size) + size) : (p % size);
}

bool clear(const Position &p, const Blizzards &b, const int turn) {
    if(p.x < 0 || p.x >= b.width || p.y < 0 || p.y >= b.height) {
        return false;
    }

    return !(b.left[p.y][mod(p.x+turn, b.width)] |
            b.right[p.y][mod(p.x-turn, b.width)] |
               b.up[p.x][mod(p.y+turn, b.height)] |
             b.down[p.x][mod(p.y-turn, b.height)]);
}

int find_shortest_path(Blizzards &b, int turn, Position start, Position finish) {
    Position pd[5] = {{0,0}, {1,0}, {0,-1}, {-1,0}, {0,1}};
    std::set<Position> exp[2];

    while(true) {
        turn++;

        if(clear(start, b, turn)) {
            exp[turn&1].insert(start);
        }

        for(auto &p : exp[(turn+1)&1]) {
            if(p == finish) {
                return turn-1;
            }

            for(int d=0; d<5; d++) {
                if(p+pd[d] == finish || clear(p+pd[d], b, turn)) {
                    exp[turn&1].insert(p+pd[d]);
                }
            }
        }

        exp[(turn+1)&1].clear();
    }

    return -1;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    Blizzards b(map[0].length()-2, map.size()-2);

    for(int y=1; y<map.size()-1; y++) {
        for(int x=1; x<map[y].length()-1; x++) {
            b.right[y-1].push_back(map[y][x] == '>');
            b.left[y-1].push_back(map[y][x] == '<');
            b.up[x-1].push_back(map[y][x] == '^');
            b.down[x-1].push_back(map[y][x] == 'v');
        }
    }

    int out = find_shortest_path(b, 0, Position(0,0), Position(b.width-1, b.height));
    std::cout << "Part1: " << out << "\n";

    int back = find_shortest_path(b, out, Position(b.width-1, b.height-1), Position(0,-1));
    std::cout << "Part2: " << find_shortest_path(b, back, Position(0,0), Position(b.width-1, b.height)) << "\n";

    return 0;
}
