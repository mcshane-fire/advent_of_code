#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <regex>

struct vertical {
    int top;
    int bottom;

    vertical(int _top, int _bottom) : top(_top), bottom(_bottom) {};
    vertical(int _top) : top(_top), bottom(_top) {};

    bool operator<(const vertical &v) const {
        if(top == v.top) {
            return bottom < v.bottom;
        }
        return top < v.top;
    }

    bool operator==(const vertical &v) const {
        return top == v.top && bottom == v.bottom;
    }

};

void print(std::vector<std::vector<vertical>> &map) {
    auto p = std::vector<std::string>();
    for(int i=0; i<1000; i++) {
        std::string line(map.size(), ' ');
        p.push_back(line);
    }

    int min_x = 1000;
    int max_x = 0;

    for(int i=0; i<map.size(); i++) {
        for(auto v : map[i]) {
            for(int y=v.top; y<=v.bottom; y++) {
                p[y][i] = '#';
                min_x = std::min(min_x, i);
                max_x = std::max(max_x, i);
            }
        }
    }

    for(int y=0; y<p.size(); y++) {
        std::cout << p[y] << "\n";
    }
}

void normalize(std::vector<std::vector<vertical>> &map) {
    for(auto &v : map) {
        std::sort(v.begin(), v.end());

        int i=1;
        while(i < v.size()) {
            if(v[i-1].bottom+1 >= v[i].top) {
                v[i-1].bottom = v[i].bottom;
                v.erase(v.begin() + i);
            } else {
                i++;
            }
        }
    }
}

int move_sideways(std::vector<std::vector<vertical>> &map, int x, int y) {
    vertical v(y);
    auto it = std::lower_bound(map[x].begin(), map[x].end(), v);
    if(it != map[x].end() && (*it).top == y) {
        return 0;
    }
    if(std::distance(map[x].begin(), it) > 0) {
        if((*std::prev(it)).bottom < y) {
            return 1;
        } else {
            return 0;
        }
    }
    return 1;
}

int drop(std::vector<std::vector<vertical>> &map, int start_x, int start_y) {
    normalize(map);
    int x = start_x;
    int y = start_y;
    while(true) {
        vertical v(y);
        auto it = std::lower_bound(map[x].begin(), map[x].end(), v);
        if(it == map[x].end()) {
            break;
        }

        if(move_sideways(map, x-1, (*it).top)) {
            x--;
            y = (*it).top;
        }
        else if(move_sideways(map, x+1, (*it).top)) {
            x++;
            y = (*it).top;
        } else {
            map[x].push_back({(*it).top-1});
            if(x == start_x && (*it).top-1 == start_y) {
                return 1;
            }
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex pair = std::regex("(\\d+),(\\d+)");
    std::smatch res;

    std::vector<std::vector<vertical>> map(1000);

    int max_y = 0;

    while(std::getline(input, line)) {
        int prev_x = -1;
        int prev_y = -1;
        while(std::regex_search(line, res, pair)) {
            int x = std::stoi(res[1]);
            int y = std::stoi(res[2]);
            if(prev_y != -1) {
                for(int i=std::min(x,prev_x); i<=std::max(x,prev_x); i++) {
                    vertical v(std::min(y, prev_y), std::max(y, prev_y));
                    if(std::find(map[i].begin(), map[i].end(), v) == map[i].end()) {
                        map[i].push_back(v);
                    }
                    max_y = std::max(max_y, std::max(y, prev_y));
                }
            }
            prev_x = x;
            prev_y = y;
            line = res.suffix().str();
        }
    }

    // part 2
    for(auto &v : map) {
        v.push_back({max_y+2, max_y+2});
    }

    int count=1;
    while(drop(map, 500, 0) == 0) {
        count++;
    }

    print(map);

    std::cout << "Count: " << count << "\n";

    return 0;
}