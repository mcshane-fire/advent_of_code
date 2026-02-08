#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include <format>
#include <algorithm>

struct pos {
    int x;
    int y;
    pos(int _x, int _y) : x(_x), y(_y) {};

    pos(const pos &a) {
        x = a.x;
        y = a.y;
    }

    bool operator<(const struct pos &a) const {
        return (x < a.x) || (x == a.x && y < a.y);
    }

    bool operator==(const struct pos &a) const {
        return (x == a.x) && (y == a.y);
    }

    friend std::ostream& operator<< (std::ostream& stream, const struct pos &p) {
        stream << "(" << p.x << "," << p.y << ")";
        return stream;
    }
};

void count_destinations_from(std::vector<std::string>& map, int steps, pos p, std::vector<int>& num) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::vector<std::set<pos>> frontier(2, std::set<pos>());

    frontier[0].insert(p);
    for(int i=0; i<steps; i++) {
        int w = i&1;

        num.push_back(frontier[w].size());
        frontier[1-w] = std::set<pos>();

        for(auto& p : frontier[w]) {
            for(int dir=0; dir<4; dir++) {
                int ny = p.y+yd[dir];
                int nx = p.x+xd[dir];
                if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length()) {
                    if(map[ny][nx] != '#') {
                        frontier[1-w].insert({nx, ny});
                    }
                }
            }
        }
    }

    num.push_back(frontier[steps&1].size());
}

int count_destinations(std::vector<std::string>& map, int steps) {
    int x, y;
    for(y=0; y<map.size(); y++) {
        if((x = map[y].find('S')) != std::string::npos) {
            break;
        }
    } 

    std::vector<int> num;
    count_destinations_from(map, steps, {x,y}, num);
    return num.back();
}

int reduce(int p, int max) {
    p %= max;
    if(p < 0) {
        p += max;
    }
    return p;
}

void print_infinite_map(std::vector<std::string>& map, std::set<pos>& frontier) {
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;


    for(auto p : frontier) {
        min_x = std::min(min_x, p.x);
        max_x = std::max(max_x, p.x);
        min_y = std::min(min_y, p.y);
        max_y = std::max(max_y, p.y);
    }

    min_x -= reduce(min_x, map[0].length());
    min_y -= reduce(min_y, map.size());

    std::vector<std::vector<int>> count(((max_y - min_y) / map.size())+1, std::vector<int>(((max_x-min_x) / map[0].length())+1));

    int seen = 0;

    for(int y=min_y; y<=max_y; y++) {
        if(reduce(y, map.size()) == 0) {
            //std::cout << "\n";

        }
        for(int x=min_x; x<=max_x; x++) {

            if(reduce(x,map[0].length()) == 0) {
                //std::cout << " | ";
            }

            if(frontier.contains({x,y})) {
                //std::cout << "X";
                seen++;
                count[(y-min_y) / map.size()][(x-min_x)/map[0].length()]++;
            } else {
                //std::cout << map[reduce(y,map.size())][reduce(x,map[0].length())];
            }

        }
        //std::cout << "\n";
    }
    std::cout << " (" << seen << ")\n";
    for(auto& v : count) {
        for(auto n : v) {
            std::cout << std::format("{:3d}", n) << " ";
        }
        std::cout << "\n";
    }
}

enum expand {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST,
    CENTER,
};

struct compass_info {
    expand dir;
    pos p;
    expand opposite;
    int subtract;
    std::vector<int> best;
    std::vector<int> cover;
    std::string name;
    std::vector<std::pair<expand,expand>> next;

    compass_info(expand _dir, pos _p, expand _opp, int _sub, std::string _name, std::vector<std::pair<expand,expand>> _next) :
            dir(_dir), p(_p), opposite(_opp), subtract(_sub), name(_name), next(_next) {
        best = std::vector<int>();
        cover = std::vector<int>();
    };
};

int min_distance_to(std::vector<std::string>& map, expand start, std::vector<compass_info>& compass, std::vector<int>& best) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::map<int,std::vector<pos>> frontier;
    std::map<pos,int> local_best;

    frontier[0] = std::vector<pos>();
    frontier[0].push_back(compass[start].p);

    while(frontier.size() > 0) {
        auto v = *frontier.begin();
        frontier.erase(v.first);

        for(auto& p : v.second) {
            if(!local_best.contains(p)) {
                local_best[p] = v.first;
                for(int dir=0; dir<4; dir++) {
                    int ny = p.y+yd[dir];
                    int nx = p.x+xd[dir];
                    if(ny >= 0 && ny < map.size() && nx >= 0 && nx < map[ny].length()) {
                        if(map[ny][nx] != '#' && !local_best.contains({nx, ny})) {
                            if(!frontier.contains(v.first+1)) {
                                frontier[v.first+1] = std::vector<pos>();
                            }
                            frontier[v.first+1].emplace_back(nx, ny);
                        }
                    }
                }
            }
        }
    }

    int max_dist = 0;

    for(int i=0; i<CENTER; i++) {
        best.push_back(local_best[compass[i].p]);
        max_dist = std::max(max_dist, local_best[compass[i].p]);
    }

    return max_dist;
}


struct grid {
    expand dir;
    expand start;
    expand end;
    int steps_remaining;

    grid(expand _dir, expand _start, expand _end, int _steps_remaining) : dir(_dir), start(_start), end(_end), steps_remaining(_steps_remaining) {};
};

int get_covered(std::vector<int>& cover, int steps) {
    int total = 0;
    if(steps < cover.size()) {
        total += cover[steps];
    }
    else {
        if(steps & 1) {
            total += cover[cover.size()-2];
        } else {
            total += cover.back();
        }
    }
    return total;
}


int64_t count_infinite_destinations(std::vector<std::string>& map, int steps) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::set<pos> frontier;
    int x, y;
    for(y=0; y<map.size(); y++) {
        if((x = map[y].find('S')) != std::string::npos) {
            break;
        }
    } 

    std::vector<compass_info> compass = {{NORTH, {x,0}, SOUTH, 1, "North", {}},
                                         {SOUTH, {x,(int) map.size()-1}, NORTH, 1, "South", {}},
                                         {EAST, {(int) map[0].length()-1,y}, WEST, 1, "East ", {}},
                                         {WEST, {0,y}, EAST, 1, "West ", {}},
                                         {NORTH_EAST, {(int) map[0].length()-1,0}, SOUTH_WEST, 2, "NEast", {{NORTH,NORTH_WEST}, {EAST,SOUTH_EAST}, {NORTH_EAST,NORTH_EAST}}},
                                         {NORTH_WEST, {0,0}, SOUTH_EAST, 2, "NWest", {{NORTH,NORTH_EAST}, {WEST,SOUTH_WEST}, {NORTH_WEST,NORTH_WEST}}},
                                         {SOUTH_EAST, {(int) map[0].length()-1,(int) map.size()-1}, NORTH_WEST, 2, "SEast", {{SOUTH,SOUTH_WEST}, {EAST,NORTH_EAST}, {SOUTH_EAST,SOUTH_EAST}}},
                                         {SOUTH_WEST, {0,(int) map.size()-1}, NORTH_EAST, 2, "SWest", {{SOUTH,SOUTH_EAST},{WEST,NORTH_WEST},{SOUTH_WEST,SOUTH_WEST}}},
                                         {CENTER, {x,y}, CENTER, 0, "Centr", {}}};

    for(auto &p : compass) {
        int m = min_distance_to(map, p.dir, compass, p.best);
        count_destinations_from(map, (m+10) & ~1, p.p, p.cover);
    }

    std::vector<grid> explore;

    for(auto &p : compass) {
        explore.emplace_back(p.dir, p.opposite, p.dir, steps - compass[CENTER].best[p.dir] - p.subtract);
    }

    int64_t total = get_covered(compass[CENTER].cover, steps);

    while(explore.size() > 0) {
        grid g = explore.back();
        explore.pop_back();
        int64_t n = get_covered(compass[g.start].cover, g.steps_remaining);

        if(g.dir == CENTER || g.steps_remaining < 0) {
            continue;
        }

        int scan, steps;

        switch(g.dir) {
            case NORTH: case EAST: case SOUTH: case WEST:
                steps = compass[g.start].best[g.end] + compass[g.dir].subtract;
                scan = (g.steps_remaining - compass[g.start].cover.size()) / (steps * 2);
                if(scan > 0) {
                    n += get_covered(compass[g.start].cover, g.steps_remaining - compass[g.start].best[g.end]-1);
                    n *= scan;
                    g.steps_remaining -= scan * steps * 2;
                }
                else {
                    g.steps_remaining -= steps;
                }
            
                explore.push_back(g);
                break;         

            case NORTH_EAST: case NORTH_WEST: case SOUTH_EAST: case SOUTH_WEST:
                for(auto&p : compass[g.dir].next) {
                    explore.emplace_back(p.first, g.start, p.second, g.steps_remaining - compass[g.start].best[p.second] - compass[p.first].subtract);
                }
                break;
        }

        total += n;        
    }

    return total;

    /*
    // slow method
    frontier.insert({x, y});
    for(int i=0; i<steps; i++) {
        std::set<pos> next_frontier;

        for(auto& p : frontier) {
            for(int dir=0; dir<4; dir++) {
                int ny = p.y+yd[dir];
                int nx = p.x+xd[dir];

                int my = reduce(ny, map.size());
                int mx = reduce(nx, map[my].length());

                if(map[my][mx] != '#') {
                    next_frontier.insert({nx, ny});
                }
            }
        }
        frontier = next_frontier;
    }

    print_infinite_map(map, frontier);

    if(frontier.size() != total) {
        return 0;
    }
    
    return frontier.size();
    */
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_destinations(map, 64) << "\n";
    std::cout << "Part2: " << count_infinite_destinations(map, 26501365) << "\n";

    return 0;
}
