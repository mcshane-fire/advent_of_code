#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>

int find_next_bus(int start_time, std::vector<std::pair<int,int>> &busses) {
    int t = start_time;
    while(true) {
        for(auto n : busses) {
            if(t % n.first == 0) {
                return n.first * (t - start_time);
            }
        }
        t++;
    }

    return 42;
}

int64_t find_cascading_departures(std::vector<std::pair<int,int>> &busses) {
    int64_t offset = 0;
    int64_t period = busses[0].first;

    for(int i=1; i<busses.size(); i++) {
        int64_t lcm = std::lcm(period, busses[i].first);
        int64_t j=0;
        while(j < lcm && (j + offset) % busses[i].first != busses[i].first - (busses[i].second % busses[i].first)) {
            j += period;
        }

        period = lcm;
        offset += j;
    }

    return offset;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::pair<int,int>> busses;

    std::getline(input, line);
    int start_time = std::stoi(line);

    std::getline(input, line);
    int offset = 0;
    while(true) {
        size_t r;
        if(line[0] == 'x') {
            r = 1;
        } else {
            busses.emplace_back(std::stoi(line, &r), offset);
        }
        if(r == line.length()) {
            break;
        }
        line = line.substr(r+1);
        offset++;
    }

    std::cout << "Part1: " << find_next_bus(start_time, busses) << "\n";
    std::cout << "Part2: " << find_cascading_departures(busses) << "\n";

    return 0;
}
