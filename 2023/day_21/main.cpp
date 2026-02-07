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

    /*
    std::cout << "count from " << p << " : ";
    for(auto n : num) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    */

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
};

void min_distance_to(std::vector<std::string>& map, pos start, std::map<expand,pos>& compass, std::map<pos,std::map<pos,int>>& best) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::map<int,std::vector<pos>> frontier;
    std::map<pos,int> local_best;

    frontier[0] = std::vector<pos>() = {start};

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

    best[start] = std::map<pos,int>();

    for(auto &p : compass) {
        best[start][p.second] = local_best[p.second];
    }

    /*
    std::cout << "\n";
    for(int y=0; y<map.size(); y++) {
        for(int x=0; x<map[y].length(); x++) {
            if(local_best.contains({x,y})) {
                std::cout << std::format("{:3d} ", local_best[{x,y}]);
            } else {
                std::cout << std::format("{:c}{:c}{:c} ", map[y][x], map[y][x], map[y][x]);
            }
        }
        std::cout << "\n";
    }*/
}

struct grid {
    expand dir;
    pos p;
    int steps_remaining;

    grid(expand _dir, pos _p, int _steps) : dir(_dir), p(_p), steps_remaining(_steps) {};
    //grid(const grid &a) : p(a.p),dir(a.dir),steps_remaining(a.steps_remaining) {};
};

int get_covered(std::map<expand,pos>& compass, std::map<pos,std::vector<int>>& cover, int steps, pos p) {
    int total = 0;
    if(steps < cover[p].size()) {
        total += cover[p][steps];
    }
    else {
        if(steps & 1) {
            total += cover[p][cover[p].size()-2];
        } else {
            total += cover[p].back();
        }
    }
    return total;
}

int count_infinite_destinations(std::vector<std::string>& map, int steps) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    std::set<pos> frontier;
    int x, y;
    for(y=0; y<map.size(); y++) {
        if((x = map[y].find('S')) != std::string::npos) {
            break;
        }
    } 

    // combine into struct?
    std::map<expand,pos> compass = {{NORTH,{x,0}}, {EAST,{(int) map[0].length()-1,y}}, {SOUTH,{x,(int) map.size()-1}}, {WEST,{0,y}},
                    {NORTH_EAST,{(int) map[0].length()-1,0}}, {NORTH_WEST,{0,0}}, {SOUTH_EAST,{(int) map[0].length()-1,(int) map.size()-1}}, {SOUTH_WEST,{0,(int) map.size()-1}}};
    std::map<expand,expand> opposite = {{NORTH,SOUTH}, {EAST,WEST}, {SOUTH,NORTH}, {WEST,EAST}, {NORTH_EAST,SOUTH_WEST}, {SOUTH_WEST,NORTH_EAST}, 
                    {NORTH_WEST,SOUTH_EAST}, {SOUTH_EAST,NORTH_WEST}};
    std::map<expand,int> subtract = {{NORTH,1}, {WEST,1}, {EAST,1}, {SOUTH,1}, {NORTH_EAST,2}, {NORTH_WEST,2}, {SOUTH_EAST,2}, {SOUTH_WEST,2}};
    std::map<pos,std::map<pos,int>> best;

    min_distance_to(map, {x,y}, compass, best);
    for(auto &p : compass) {
        min_distance_to(map, p.second, compass, best);
    }

    std::map<pos,std::vector<int>> cover;

    for(auto&m : best) {
        cover[m.first] = std::vector<int>();
        int max = (*std::max_element(m.second.begin(), m.second.end(), [](const std::pair<pos,int> &a, const std::pair<pos,int> &b) { return a.second < b.second; })).second;
        count_destinations_from(map, (max+10) & ~1, m.first, cover[m.first]);
    }

    std::vector<grid> explore;

    for(auto &p : opposite) {
        explore.emplace_back(p.first, compass.at(p.second), steps - best[{x,y}][compass.at(p.first)] - subtract.at(p.first));
    }

    std::map<expand,std::string> smap = {{NORTH,"North"}, {EAST,"East"}, {SOUTH,"South"}, {WEST,"West"},
                            {NORTH_EAST,"North east"}, {NORTH_WEST,"North west"}, {SOUTH_EAST, "South east"}, {SOUTH_WEST,"South west"}};

    int total = get_covered(compass, cover, steps, {x,y});

    //std::cout << "\nReady\n";

    while(explore.size() > 0) {
        grid g = explore.back();
        explore.pop_back();

        //std::cout << "Explore " << smap[g.dir] << " " << g.p << " " << g.steps_remaining << " total: " << total;

        if(g.steps_remaining < 0) {
            //std::cout << "\n";
            continue;
        }

        int n = get_covered(compass, cover, g.steps_remaining, g.p);

        //std::cout << " + " << n << " = " << (total+n) << "\n";
        total += n;


        switch(g.dir) { 
            case NORTH:
            explore.emplace_back(g.dir, g.p, g.steps_remaining - best[g.p][pos(g.p.x, 0)]-1);
            break;         

            case EAST:
            explore.emplace_back(g.dir, g.p, g.steps_remaining - best[g.p][pos(map[0].length()-1, g.p.y)]-1);
            break;         

            case SOUTH:
            explore.emplace_back(g.dir, g.p, g.steps_remaining - best[g.p][pos(g.p.x, map.size()-1)]-1);
            break;         

            case WEST:
            explore.emplace_back(g.dir, g.p, g.steps_remaining - best[g.p][pos(0, g.p.y)]-1);
            break;         

            case NORTH_EAST:
            explore.emplace_back(NORTH, g.p, g.steps_remaining - best[g.p][compass.at(NORTH_WEST)] - 1);
            explore.emplace_back(EAST, g.p, g.steps_remaining - best[g.p][compass.at(SOUTH_EAST)] - 1);
            explore.emplace_back(NORTH_EAST, g.p, g.steps_remaining - best[g.p][compass.at(NORTH_EAST)] - 2);
            break;

            case NORTH_WEST:
            explore.emplace_back(NORTH, g.p, g.steps_remaining - best[g.p][compass.at(NORTH_EAST)] - 1);
            explore.emplace_back(WEST, g.p, g.steps_remaining - best[g.p][compass.at(SOUTH_WEST)] - 1);
            explore.emplace_back(NORTH_WEST, g.p, g.steps_remaining - best[g.p][compass.at(NORTH_WEST)] - 2);
            break;

            case SOUTH_WEST:
            explore.emplace_back(SOUTH, g.p, g.steps_remaining - best[g.p][compass.at(SOUTH_EAST)] - 1);
            explore.emplace_back(WEST, g.p, g.steps_remaining - best[g.p][compass.at(NORTH_WEST)] - 1);
            explore.emplace_back(SOUTH_WEST, g.p, g.steps_remaining - best[g.p][compass.at(SOUTH_WEST)] - 2);
            break;

            case SOUTH_EAST:
            explore.emplace_back(SOUTH, g.p, g.steps_remaining - best[g.p][compass.at(SOUTH_WEST)] - 1);
            explore.emplace_back(EAST, g.p, g.steps_remaining - best[g.p][compass.at(NORTH_EAST)] - 1);
            explore.emplace_back(SOUTH_EAST, g.p, g.steps_remaining - best[g.p][compass.at(SOUTH_EAST)] - 2);
            break;
        }
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
    
    return frontier.size();*/
    
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << count_destinations(map, 64) << "\n";

    /*
    for(int i=1; i<=100; i++) {
        int n = count_infinite_destinations(map, i);
        if(n == 0) {
            break;
        }
        std::cout << i << "  " << n << "\n";
    }*/

    std::cout << "Part2: " << count_infinite_destinations(map, 100) << "\n";

    return 0;
}


