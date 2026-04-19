#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

class Pos {
    public:
    Pos(std::string line) {
        size_t s;
        x[0] = std::stoi(line, &s);
        line = line.substr(s+1);
        x[1] = std::stoi(line, &s);
        line = line.substr(s+1);
        x[2] = std::stoi(line);
    }

    Pos(int _x, int _y, int _z) {
        x[0] = _x;
        x[1] = _y;
        x[2] = _z;
    }

    Pos operator+(const Pos &p) const {
        return Pos(x[0]+p.x[0], x[1]+p.x[1], x[2]+p.x[2]);
    }

    Pos operator-(const Pos &p) const {
        return Pos(x[0]-p.x[0], x[1]-p.x[1], x[2]-p.x[2]);
    }

    bool operator==(const Pos &p) const {
        return x[0] == p.x[0] && x[1] == p.x[1] && x[2] == p.x[2];
    }

    bool operator<(const Pos &p) const {
        return x[0] == p.x[0] ? (x[1] == p.x[1] ? x[2] < p.x[2] : (x[1] < p.x[1])) : x[0] < p.x[0];
    }

    friend std::ostream& operator<<(std::ostream &os, const Pos &p) {
        os << p.x[0] << "," << p.x[1] << "," << p.x[2];
        return os;
    }

    int dist(const Pos &p) {
        return std::abs(x[0] - p.x[0]) + std::abs(x[1] - p.x[1]) + std::abs(x[2] - p.x[2]);
    }

    Pos map(int map) const {
        const int mapping[24][3][2] = {
            // looking down x
            {{0,1}, {1,1}, {2,1}},
            {{0,1}, {2,1}, {1,-1}},
            {{0,1}, {1,-1}, {2,-1}},
            {{0,1}, {2,-1}, {1,1}},
            // looking down y
            {{1,1}, {0,1}, {2,-1}},
            {{1,1}, {2,1}, {0,1}},
            {{1,1}, {0,-1}, {2,1}},
            {{1,1}, {2,-1}, {0,-1}},
            // looking down z
            {{2,1}, {1,1}, {0,-1}},
            {{2,1}, {0,-1}, {1,-1}},
            {{2,1}, {1,-1}, {0,1}},
            {{2,1}, {0,1}, {1,1}},

            // looking down -x
            {{0,-1}, {1,1}, {2,-1}},
            {{0,-1}, {2,1}, {1,1}},
            {{0,-1}, {1,-1}, {2,1}},
            {{0,-1}, {2,-1}, {1,-1}},
            // looking down -y
            {{1,-1}, {0,1}, {2,1}},
            {{1,-1}, {2,1}, {0,-1}},
            {{1,-1}, {0,-1}, {2,-1}},
            {{1,-1}, {2,-1}, {0,1}},
            // looking down -z
            {{2,-1}, {1,1}, {0,1}},
            {{2,-1}, {0,1}, {1,-1}},
            {{2,-1}, {1,-1}, {0,-1}},
            {{2,-1}, {0,-1}, {1,1}}};

        return Pos(
            x[mapping[map][0][0]] * mapping[map][0][1],
            x[mapping[map][1][0]] * mapping[map][1][1],
            x[mapping[map][2][0]] * mapping[map][2][1]);
    }

    private:
    int x[3];
};

class Scanner {
    public:
    Scanner(int _num) : num(_num) {
        comb.emplace_back(0, 0, 0);
    };

    void add(std::string line) {
        Pos p = Pos(line);
        data.insert(p);
    }

    std::pair<int,Pos> find_relationship(const Scanner &s) {
        std::vector<std::map<Pos,int>> diff;
        for(int m=0; m<24; m++) {
            diff.push_back(std::map<Pos,int>());
            for(auto &md : data) {
                for(auto &od : s.data) {
                    diff[m][md - od.map(m)]++;
                }
            }
            auto it = std::max_element(diff[m].begin(), diff[m].end(), [](const auto &a, const auto &b){ return a.second < b.second; });
            if((*it).second >= 12) {
                return {m, (*it).first};
            }
        }

        return {-1,Pos(0,0,0)};
    }

    void combine(Scanner &s, int map, Pos diff) {
        for(auto &od : s.data) {
            Pos np = od.map(map) + diff;
            data.insert(np);
        }
        comb.push_back(diff);
    }

    int num;
    std::set<Pos> data;
    std::vector<Pos> comb;
};

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Scanner> scanners;

    while(std::getline(input, line)) {
        if(line.length() > 0) {
            if(line[1] == '-') {
                line = line.substr(12);
                scanners.emplace_back(std::stoi(line));
            } else {
                scanners.back().add(line);
            }
        }
    }

    int max_turns = scanners.size();
    while(scanners.size() > 1 && --max_turns > 0) {
        for(int i=1; i<scanners.size(); i++) {
            auto p = scanners[0].find_relationship(scanners[i]);
            //std::cout << scanners.size() << " " << i << ": " << p.first << " " << p.second << "\n";
            if(p.first >= 0) {
                scanners[0].combine(scanners[i], p.first, p.second);
                scanners.erase(scanners.begin() + i);
                break;
            }
        }
    }

    std::cout << "Part1: " << scanners[0].data.size() << "\n";

    int max_dist = 0;
    for(int i=0; i<scanners[0].comb.size()-1; i++) {
        for(int j=0; j<scanners[0].comb.size(); j++) {
            max_dist = std::max(max_dist, scanners[0].comb[i].dist(scanners[0].comb[j]));
        }
    }

    std::cout << "Part2: " << max_dist << "\n";


    return 0;
}
