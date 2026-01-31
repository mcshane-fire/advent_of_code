#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>

struct pos {
    int x, y, z;
};

std::pair<int,int> make_fall(std::vector<std::vector<struct pos>>& bricks) {
    int x_max = 0;
    int y_max = 0;

    for(auto &b : bricks) {
        for(auto &p : b) {
            x_max = std::max(x_max, p.x);
            y_max = std::max(y_max, p.y);
        }
    }

    std::vector<std::vector<std::map<int,int>>> map(x_max+1, std::vector<std::map<int,int>>(y_max+1));
    std::vector<std::vector<int>> above(bricks.size());
    std::vector<int> below(bricks.size());

    for(int i=0; i<bricks.size(); i++) {
        for(auto&p : bricks[i]) {
            map[p.x][p.y][p.z] = i;
        }
    }

    for(int x=0; x<=x_max; x++) {
        for(int y=0; y<=y_max; y++) {
            for(auto it = map[x][y].begin(); it != map[x][y].end();) {
                auto nit = it++;
                if(it != map[x][y].end() && (*nit).second != (*it).second) {
                    above[(*nit).second].push_back((*it).second);
                    below[(*it).second]++;
                }
            }
        }
    }

    std::vector<int> to_fall;
    std::vector<std::vector<std::pair<int,int>>> floor(x_max+1, std::vector<std::pair<int,int>>(y_max+1));
    std::vector<std::set<int>> supports(bricks.size());
    std::vector<std::set<int>> supported_by(bricks.size());


    for(int i=0; i<above.size(); i++) {
        if(below[i] == 0) {
            to_fall.push_back(i);
        }
    }

    while(to_fall.size() > 0) {
        int b = to_fall.back();
        to_fall.pop_back();

        int dist = INT_MAX;
        for(auto &p : bricks[b]) {
            dist = std::min(dist, p.z - floor[p.x][p.y].first);
        }

        for(auto &p : bricks[b]) {
            p.z -= dist-1;
            if(floor[p.x][p.y].first > 0 && floor[p.x][p.y].first == p.z-1 && floor[p.x][p.y].second != b) {
                //std::cout << " block " << b << " supported by " << floor[p.x][p.y].second << "\n";
                supports[floor[p.x][p.y].second].insert(b);
                supported_by[b].insert(floor[p.x][p.y].second);
            }
            floor[p.x][p.y].first = std::max(floor[p.x][p.y].first, p.z);
            floor[p.x][p.y].second = b;
        }

        for(auto i : above[b]) {
            below[i]--;
            if(below[i] == 0) {
                to_fall.push_back(i);
            }
        }
    }

    int destroyable = 0;

    auto it = supports.begin();
    auto it2 = supported_by.begin();
    for(auto &b : bricks) {
        /*
        std::cout << "brick: ";
        for(auto &p : b) {
            std::cout << p.x << "," << p.y << "," << p.z << "  ";
        }
        std::cout << " Supports: ";
        for(auto p : (*it)) {
            std::cout << p << " ";
        }
        std::cout << " Supported by: ";
        for(auto p : (*it2)) {
            std::cout << p << " ";
        }*/

        bool destroy = true;
        for(auto p : (*it)) {
            if(supported_by[p].size() == 1) {
                destroy = false;
            }
        }
        if(destroy) {
            destroyable++;
            //std::cout << " Safe to destroy";
        }
        it++;
        it2++;
        //std::cout << "\n";
    }    

    std::vector<int> causes(bricks.size());
    int chain_reactions = 0;

    for(int i=0; i<bricks.size(); i++) {
        //std::cout << "Starting with brick " << i << "\n";

        std::set<int> removed = {i};
        std::vector<int> removing = {i};
        while(removing.size() > 0) {
            int r = removing.back();
            removing.pop_back();
            removed.insert(r);

            //std::cout << " - remove brick " << r << "\n";

            for(auto p : supports[r]) {
                if(!removed.contains(p)) {
                    //std::cout << "   - removing support on brick " << p << "\n";
                    bool safe = false;
                    for(auto n : supported_by[p]) {
                        //std::cout << "     : supported by " << n << (removed.contains(n) ? " which has been removed" : " which is still there") << "\n";
                        if(!removed.contains(n)) {
                            safe = true;
                            break;
                        }
                    }
                    //std::cout << "   - this is " << (safe ? "safe" : "now falling") << "\n";
                    if(!safe) {
                        removing.push_back(p);
                    }
                }
            }
        }
        //std::cout << ": finished analysis: " << removed.size() << " to fall\n";
        chain_reactions += removed.size() - 1;
    }

    return {destroyable, chain_reactions};
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> map;
    std::regex desc = std::regex("^(\\d+),(\\d+),(\\d+)~(\\d+),(\\d+),(\\d+)$");
    std::smatch res;

    std::vector<std::vector<struct pos>> bricks;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, desc)) {
            std::vector<struct pos> brick;
            for(int x=std::stoi(res[1]); x<=std::stoi(res[4]); x++) {
                for(int y=std::stoi(res[2]); y<=std::stoi(res[5]); y++) {
                    for(int z=std::stoi(res[3]); z<=std::stoi(res[6]); z++) {
                        brick.emplace_back(x, y, z);
                    }
                }
            }
            bricks.emplace_back(brick);
        }
    }

    auto [a,b] = make_fall(bricks);
    std::cout << "Part1: " << a << "\n";
    std::cout << "Part2: " << b << "\n";

    return 0;
}


