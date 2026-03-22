#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <set>
#include <map>

struct Pos {
    int x = 0;
    int y = 0;

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

void print_elves(const std::set<Pos> &elves) {
    auto it = elves.begin();
    int x_min = (*it).x;
    int x_max = (*it).x;
    int y_min = (*it).y;
    int y_max = (*it).y;
    while(it != elves.end()) {
        x_min = std::min(x_min, (*it).x);
        x_max = std::max(x_max, (*it).x);
        y_min = std::min(y_min, (*it).y);
        y_max = std::max(y_max, (*it).y);
        it++;
    }
    auto view = std::vector<std::string>(y_max-y_min+1, std::string(x_max-x_min+1, '.'));
    for(auto &p : elves) {
        view[p.y - y_min][p.x-x_min] = '#';
    }
    std::cout << "\n";
    for(auto &s : view) {
        std::cout << s << "\n";
    }

}

int expand(std::vector<std::string> map, int max_rounds) {
    std::set<Pos> elves;
    Pos pd[] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}};
    int turn = 0;
    int ind[] = {0, 4, 6, 2};

    for(int y=0; y<map.size(); y++) {
        for(int x=0; x<map[y].length(); x++) {
            if(map[y][x] == '#') {
                elves.insert(Pos(x, y));
            }
        }
    }

    //print_elves(elves);

    bool go_again = true;
    while(go_again && (max_rounds == 0 || turn < max_rounds)) {
        go_again = false;
        auto proposed = std::map<Pos,Pos>();

        for(auto &p : elves) {
            bool adj[9];
            bool all = false;
            for(int i=0; i<8; i++) {
                adj[i] = elves.contains(p + pd[i]);
                all |= adj[i];
            }
            adj[8] = adj[0];

            if(!all) {
                continue;
            }


            for(int d = 0; d<4; d++) {
                if(!(adj[ind[(d+turn)&3]] | adj[ind[(d+turn)&3]+1] | adj[ind[(d+turn)&3]+2])) {
                    go_again = true;
                    Pos np = p + pd[ind[(d+turn)&3]+1];
                    if(proposed.contains(np)) {
                        proposed.erase(np);
                    } else {
                        proposed[np] = p;
                    }
                    break;
                }
            }
        }

        for(auto &pp : proposed) {
            elves.erase(pp.second);
            elves.insert(pp.first);
        }

        //print_elves(elves);

        turn++;
    }

    if(max_rounds == 0) {
        return turn;
    }

    auto it = elves.begin();
    int x_min = (*it).x;
    int x_max = (*it).x;
    int y_min = (*it).y;
    int y_max = (*it).y;
    while(it != elves.end()) {
        x_min = std::min(x_min, (*it).x);
        x_max = std::max(x_max, (*it).x);
        y_min = std::min(y_min, (*it).y);
        y_max = std::max(y_max, (*it).y);
        it++;
    }
    return ((x_max-x_min+1) * (y_max-y_min+1)) - elves.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input2.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << expand(map, 10) << "\n";
    std::cout << "Part2: " << expand(map, 0) << "\n";

    return 0;
}
