#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <regex>

class Board {
    public:
    void add_wall(int x1, int y1, int x2, int y2) {
        for(int x=std::min(x1,x2); x<=std::max(x1,x2); x++) {
            for(int y=std::min(y1,y2); y<=std::max(y1,y2); y++) {
                safe(x,y);
                cols[x][y] = 1;
            }
        }        
    }

    int drop(int start_x, int start_y) {
        int x = start_x;
        int y = start_y;
        while(true) {
            safe(x,y+1);
            safe(x-1,y+1);
            safe(x+1,y+1);

            if(cols[x][y+1] == 0) {
                if(cols[x].size() == y+2) {
                    return 1;
                }
                y++;
            } else if(cols[x-1][y+1] == 0) {
                x--;
                y++;
            } else if(cols[x+1][y+1] == 0) {
                x++;
                y++;
            } else {
                cols[x][y] = 2;
                if(x == start_x && y == start_y) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }           
    }

    void print() {
        for(auto &v : cols) {
            for(auto i : v.second) {
                std::cout << (i == 0 ? '.' : (i == 1 ? '#' : 'o'));
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
    private:
    void safe(int x, int y) {
        if(!cols.contains(x)) {
            cols[x] = std::vector<int>();
        }
        if(cols[x].size() < y+1) {
            cols[x].resize(y+1,0);
        }
    }

    std::map<int,std::vector<int>> cols;
};

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex pair = std::regex("(\\d+),(\\d+)");
    std::smatch res;

    Board b;
    int max_y = 0;

    while(std::getline(input, line)) {
        int prev_x = -1;
        int prev_y = -1;
        while(std::regex_search(line, res, pair)) {
            int x = std::stoi(res[1]);
            int y = std::stoi(res[2]);
            if(prev_y != -1) {
                b.add_wall(prev_x, prev_y, x, y);
            }
            max_y = std::max(max_y, std::max(y, prev_y));
            prev_x = x;
            prev_y = y;
            line = res.suffix().str();
        }
    }

    Board b2 = b;

    int count=0;
    while(b.drop(500, 0) == 0) {
        count++;
    }
    std::cout << "Part1: " << count << "\n";

    b2.add_wall(500 - (max_y+10), max_y+2, 500 + (max_y+10), max_y+2);
    count=1;
    while(b2.drop(500, 0) == 0) {
        //b2.print();
        count++;
    }
    std::cout << "Part2: " << count << "\n";

    return 0;
}
