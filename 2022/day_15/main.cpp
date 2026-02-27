#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <regex>

class Recording {
    public:
    Recording(int sx, int sy, int bx, int by) : sensor_x(sx), sensor_y(sy), beacon_x(bx), beacon_y(by) {}

    void cover(std::vector<std::pair<int,int>> &cov, int row) {
        int md = std::abs(sensor_x - beacon_x) + std::abs(sensor_y - beacon_y);
        int diff = std::abs(sensor_y - row);
        if(diff > md) {
            return;
        }
        cov.push_back({sensor_x - (md-diff), ((md - diff) * 2) + 1});      
    }

    int sensor_x;
    int sensor_y;
    int beacon_x;
    int beacon_y;
};

int64_t count_covered(std::vector<Recording> &list, int row, int max = -1) {
    std::vector<std::pair<int,int>> cov;
    std::set<int> avoid;

    for(auto &r : list) {
        r.cover(cov, row);
        if(r.beacon_y == row) {
            avoid.insert(r.beacon_x);
        }
    }

    std::sort(cov.begin(), cov.end());

    int64_t total = 0;
    int x = max == -1 ? cov[0].first : 0;

    for(auto &p : cov) {
        if(p.first > x) {
            x = p.first;

            if(max != -1 && p.first > 0 && p.first <= max) {
                return p.first-1;
            }
        }
        int nx = std::max(x, p.first+p.second);

        if(max != -1) {
            nx = std::min(nx, max);
        }
        else {
            auto it = std::lower_bound(avoid.begin(), avoid.end(), x);
            while(it != avoid.end() && (*it) < nx) {
                total--;
                it++;
            }
        }

        total += nx - x;
        x = nx;
    }  

    return total;
}

int64_t find_single(std::vector<Recording> &list, int max) {
    int total = 0;
    for(int64_t y=0; y<max; y++) {
        int64_t x = count_covered(list, y, max);
        if(x != max) {
            return (x * 4000000) + y;
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex reg = std::regex("^Sensor at x=([-\\d]+), y=([-\\d]+): closest beacon is at x=([-\\d]+), y=([-\\d]+)$");
    std::smatch res;

    std::vector<Recording> list;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, reg)) {
            list.push_back({std::stoi(res[1]), std::stoi(res[2]), std::stoi(res[3]), std::stoi(res[4])});
        }
    }

    std::cout << "Part1: " << count_covered(list, argc >= 2 ? 2000000 : 10) << "\n";
    std::cout << "Part2: " << find_single(list, argc >= 2 ? 4000000 : 20) << "\n";

    return 0;
}
