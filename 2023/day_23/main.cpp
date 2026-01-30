#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>

struct pos {
    int x;
    int y;
    std::string path;
    int prev_x;
    int prev_y;
    int length;
};

int longest_path(std::vector<std::string>& map) {
    int longest = 0;
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};
    std::map<char,int> slope = {{'>',0}, {'^',1}, {'<',2}, {'v',3}, {'#',-1}};
    std::vector<struct pos> frontier;
    std::vector<std::vector<std::set<std::string>>> visited(map.size(),std::vector<std::set<std::string>>(map[0].length()));

    frontier.push_back({1, 0, "S", 0, 0, 0});
    while(frontier.size() > 0) {
        auto &p = frontier.back();
        frontier.pop_back();

        //std::cout << "going from " << p.x << "," << p.y << "," << p.path << "\n";
        auto it = std::find_if(visited[p.y][p.x].begin(), visited[p.y][p.x].end(),
                [p](const std::string& path) { return path.length() <= p.path.length() && 
                                                p.path.substr(0,path.length()) == path; });
        if(it != visited[p.y][p.x].end()) {
            //std::cout << " - found subset visited:" << (*it) << " current " << p.path << "\n";
            continue;
        }        

        visited[p.y][p.x].insert(p.path);

        if(p.y == map.size()-1 && p.x == map[p.y].length()-2) {
            longest = std::max(longest, p.length);
            //std::cout << "reached end with " << p.path << " length " << p.length << "\n";
            continue;
        }

        std::vector<struct pos> options;
        for(int dir=0; dir<4; dir++) {
            int ny = p.y+yd[dir];
            int nx = p.x+xd[dir];
            if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length() && !(ny == p.prev_y && nx == p.prev_x)) {
                if(map[ny][nx] == '.' || slope[map[ny][nx]] == dir) {
                    options.push_back({nx, ny, p.path, p.x, p.y, p.length+1});
                    //std::cout << " - option to go to " << p.x << "," << p.y << "\n";
                }
            }
        }
        if(options.size() > 1) {
            for(int i=0; i<options.size(); i++) {
                std::string ex(1, '0'+i);   
                frontier.push_back({options[i].x, options[i].y,
                                    options[i].path.append(ex), p.x, p.y, options[i].length});
            }
        } else if(options.size() == 1) {
            frontier.push_back(options[0]);
        }
    }

    return longest;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << longest_path(map) << "\n";

    return 0;
}


