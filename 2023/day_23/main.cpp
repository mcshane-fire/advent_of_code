#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>

struct pos {
    int x;
    int y;
    int prev_x;
    int prev_y;
    int length;
};

struct node {
    int x;
    int y;
    std::vector<std::pair<int,int>> next;
    bool last;
};

int explore_from(std::vector<struct node>& nodes, int from, int length, std::set<int>& path) {
    if(nodes[from].last) {
        //std::cout << "Reached last in " << length << "\n";
        return length;
    }
    //std::cout << "exploring from " << from << " in " << length << "\n";
    int max_length = 0;
    for(auto &p : nodes[from].next) {
        if(!path.contains(p.first)) {
            //std::cout << " - going to " << p.first << "\n";
            path.insert(from);
            max_length = std::max(max_length, explore_from(nodes, p.first, length + p.second, path));
            path.erase(from);
        }
    }

    return max_length;
}

int find_nodes(std::vector<std::string>& map, bool slippery = true) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};
    std::map<char,int> slope = {{'>',0}, {'^',1}, {'<',2}, {'v',3}, {'#',-1}};
    std::set<char> walkable = {'.'};
    std::vector<struct node> nodes;
    std::vector<int> explore;

    if(!slippery) {
        walkable.insert('<');
        walkable.insert('^');
        walkable.insert('v');
        walkable.insert('>');
    }   

    nodes.push_back({1,0,std::vector<std::pair<int,int>>(), false});
    explore.push_back(0);

    while(explore.size() > 0) {
        auto n = explore.back();
        explore.pop_back();

        //std::cout << "Exploring from node " << n << " at " << nodes[n].x << "," << nodes[n].y << "\n";

        std::vector<struct pos> options;
        for(int dir=0; dir<4; dir++) {
            int ny = nodes[n].y + yd[dir];
            int nx = nodes[n].x + xd[dir];
            if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length()) {
                if(walkable.contains(map[ny][nx]) || slope[map[ny][nx]] == dir) {
                    options.push_back({nx, ny, nodes[n].x, nodes[n].y, 1});
                }
            }
        }

        for(auto p : options) {
            while(true) {
                if(p.y == map.size()-1 && p.x == map[p.y].length()-2) {
                    break;
                }
                if(p.x == 1 && p.y == 0) {
                    break;
                }

                std::vector<struct pos> next_options;
                for(int dir=0; dir<4; dir++) {
                    int ny = p.y + yd[dir];
                    int nx = p.x + xd[dir];
                    if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length() && !(ny == p.prev_y && nx == p.prev_x)) {
                        if(walkable.contains(map[ny][nx]) || slope[map[ny][nx]] == dir) {
                            next_options.push_back({nx, ny, p.x, p.y, p.length+1});
                        }
                    }
                }
                if(next_options.size() > 1) {
                    break;
                }
                if(next_options.size() == 0) {
                    p.length = 0;
                    break;
                }
                p = next_options[0];
            }
            if(p.length > 0) {
                //std::cout << "  Found route to " << p.x << "," << p.y << " length " << p.length << "\n";
                auto it = std::find_if(nodes.begin(), nodes.end(), [&p](const struct node& n){ return n.x == p.x && n.y == p.y; });
                if(it == nodes.end()) {
                    //std::cout << "  - adding new node " << nodes.size() << "\n";
                    explore.push_back(nodes.size());
                    nodes[n].next.emplace_back(nodes.size(), p.length);
                    nodes.emplace_back(p.x, p.y, std::vector<std::pair<int,int>>(), p.y == map.size()-1 && p.x == map[p.y].length()-2);
                } else {
                    //std::cout << "  - found node " << (it - nodes.begin()) << "\n";
                    nodes[n].next.emplace_back(it - nodes.begin(), p.length);
                }
            }
        }
    }

    std::set<int> path;
    return explore_from(nodes, 0, 0, path);
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << find_nodes(map) << "\n";
    std::cout << "Part2: " << find_nodes(map, false) << "\n";

    return 0;
}
