#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

struct Pos {
    int x = 0;
    int y = 0;

    //0:   0     1     2
    //1:      0     1     2
    //2:   0     1     2 
    //3:      0     1     2

    auto operator<=>(const Pos &p) const = default;

    void move(std::string ins, int &used) {
        if(ins[0] == 'e') {
            used = 1;
            x++;
        } else if(ins[0] == 'w') {
            used = 1;
            x--;
        } else {
            int ver = ins[0] == 's' ? 1 : -1;
            int hor = ins[1] == 'e' ? 1 : -1;
            used = 2;
            if(((y & 1) == 1 && hor == -1) || ((y & 1) == 0 && hor == 1)) {
                hor = 0;
            }
            x += hor;
            y += ver;
        }
    }

    friend std::ostream& operator<<(std::ostream &os, const Pos &p) {
        os << p.x << "," << p.y;
        return os;
    }
};

int count_black(std::vector<std::string> &paths) {
    std::set<Pos> black;

    for(auto s : paths) {
        Pos p = Pos(0,0);
        while(s.length() > 0) {
            int used;
            p.move(s, used);
            s = s.substr(used);
        }
        if(black.contains(p)) {
            black.erase(p);
        } else {
            black.insert(p);
        }
    }

    return black.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::string> paths;

    int64_t total = 0;
    while(std::getline(input, line)) {
        paths.push_back(line);
    }

    std::cout << "Part1: " << count_black(paths) << "\n";

    return 0;
}
