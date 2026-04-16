#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <algorithm>

struct Pos {
    int x = 0;
    int y = 0;

    bool inmap(std::vector<std::string> &map) {
        return y >= 0 && y < map.size() && x >= 0 && x < map[y].length();        
    }

    Pos operator+(const Pos &p) const {
        return {x+p.x, y+p.y};
    }

    bool operator==(const Pos &p) const {
        return x == p.x && y == p.y;
    }

    bool operator<(const Pos &p) const {
        return x == p.x ? (y < p.y) : (x < p.x);
    }
};

void print_points(std::set<Pos> &points) {
    int ymax = (*points.rbegin()).y;
    int xmax = (*std::max_element(points.begin(), points.end(), [](const Pos &a, const Pos &b) { return a.x < b.x; })).x;

    auto map = std::vector<std::string>(ymax+1, std::string(xmax+1, '.'));
    for(auto &p : points) {
        map[p.y][p.x] = '#';
    }

    for(auto &l : map) {
        std::cout << l << "\n";
    }
}

int count_visible(std::set<Pos> points, std::vector<std::pair<char,int>> &folds, int num) {

    std::set<Pos> new_points;
    for(int i=0; i<std::min(num, (int) folds.size()); i++) {

        for(auto p : points) {
            if(folds[i].first == 'x') {
                if(p.x > folds[i].second) {
                    p.x = (2 * folds[i].second) - p.x;
                }
            } else {
                if(p.y > folds[i].second) {
                    p.y = (2 * folds[i].second) - p.y;
                }
            }
            new_points.insert(p);
        }

        points.clear();
        new_points.swap(points);
    }

    if(num == INT_MAX) {
        print_points(points);
    }

    return points.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::set<Pos> points;
    std::vector<std::pair<char,int>> folds;

    while(std::getline(input, line)) {
        if(line.length() > 0) {
            auto it = std::find(line.begin(), line.end(), ',');
            if(it != line.end()) {
                int n1 = std::stoi(line.substr(0, std::distance(line.begin(), it)));
                int n2 = std::stoi(line.substr(std::distance(line.begin(), std::next(it))));
                points.insert({n1,n2});
            }
            else {
                it = std::find(line.begin(), line.end(), '=');
                char a = *std::prev(it);
                int n = std::stoi(line.substr(std::distance(line.begin(), std::next(it))));
                folds.emplace_back(a, n);
            }
        }
    }

    std::cout << "Part1: " << count_visible(points, folds, 1) << "\n";
    std::cout << "Part2:\n";
    count_visible(points, folds, INT_MAX);

    return 0;
}
