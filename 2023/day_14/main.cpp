#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

int measure_load(std::vector<std::string>& map) {
    int total = 0;

    //std::cout << "\n";
    for(int y=0; y<map.size(); y++) {
        //std::cout << map[y] << "\n";
        total += (map.size() - y) * std::count_if(map[y].begin(), map[y].end(), [](char c) { return c == 'O'; });
    }

    return total;
}

void tilt_west(std::vector<std::string>& map) {
    for(auto& line : map) {
        auto wit = line.begin();
        for(auto rit = line.begin(); rit != line.end(); rit++) {
            if((*rit) == '#') {
                wit = rit+1;
            } else if((*rit) == 'O') {
                (*rit) = '.';
                (*wit) = 'O';
                wit++;
            }
        }
    }
}

void tilt_east(std::vector<std::string>& map) {
    for(auto& line : map) {
        auto wit = line.rbegin();
        for(auto rit = line.rbegin(); rit != line.rend(); rit++) {
            if((*rit) == '#') {
                wit = rit+1;
            } else if((*rit) == 'O') {
                (*rit) = '.';
                (*wit) = 'O';
                wit++;
            }
        }
    }
}

void tilt_north(std::vector<std::string>& map) {
    for(int i=0; i<map[0].length(); i++) {
        auto wit = map.begin();
        for(auto rit = map.begin(); rit != map.end(); rit++) {
            if((*rit)[i] == '#') {
                wit = rit+1;
            } else if((*rit)[i] == 'O') {
                (*rit)[i] = '.';
                (*wit)[i] = 'O';
                wit++;
            }
        }
    }
}

void tilt_south(std::vector<std::string>& map) {
    for(int i=0; i<map[0].length(); i++) {
        auto wit = map.rbegin();
        for(auto rit = map.rbegin(); rit != map.rend(); rit++) {
            if((*rit)[i] == '#') {
                wit = rit+1;
            } else if((*rit)[i] == 'O') {
                (*rit)[i] = '.';
                (*wit)[i] = 'O';
                wit++;
            }
        }
    }
}

int measure_load_after_cycles(std::vector<std::string>& map, int cycles) {
    std::vector<int> history;
    std::vector<int> values;
    int lowest = -1;
    int start = 0;
    int cycle = -1;
    int prediction = -1;

    for(int i=0; i<1000; i++) {
        int hash = 0;
        tilt_north(map);
        hash = ((hash + measure_load(map)) * 17) & 0xffff;
        tilt_west(map);
        hash = ((hash + measure_load(map)) * 17) & 0xffff;
        tilt_south(map);
        hash = ((hash + measure_load(map)) * 17) & 0xffff;
        tilt_east(map);
        hash = ((hash + measure_load(map)) * 17) & 0xffff;

        values.push_back(measure_load(map));

        //std::cout << i << " " << measure_load(map) << " " << hash << "\n";

        auto it = std::find(history.begin(), history.end(), hash);
        if(it != history.end()) {
            //std::cout << " find -> " << (it - history.begin()) << "\n";
            if(lowest == -1) {
                start = i;
                lowest = it - history.begin();
            } else if (lowest == it - history.begin()) {
                if(cycle != -1) {
                    if(cycle == i-start) {
                        //std::cout << " matching cycle length\n";
                        break;
                    } else {
                        std::cout << " failed prediction match: " << cycle << " vs " << (i-start) << "\n";
                    }
                }
                cycle = i-start;
                prediction = values[start + ((cycles-i-1) % (i-start))];
                start = i;
            } else if(cycle != -1) {
                if(hash != history[i-cycle]) {
                    std::cout << " failed check hash: " << hash << " vs. " << history[i-cycle] << "\n";
                }
            }
        }
        history.push_back(hash);
    }

    return prediction;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    tilt_north(map);
    std::cout << "Part1: " << measure_load(map) << "\n";

    tilt_west(map);
    tilt_south(map);
    tilt_east(map);
    std::cout << "Part2: " << measure_load_after_cycles(map, 1000000000-1) << "\n";

    return 0;
}
