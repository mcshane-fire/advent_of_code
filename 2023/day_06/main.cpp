#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <sstream>

uint64_t find_number_solutions(std::vector<std::pair<uint64_t,uint64_t>>& races) {
    uint64_t total = 1;

    for(auto race : races) {
        //std::cout << race.first << "," << race.second << ": ";
        if(4 * race.second > race.first * race.first) {
            //std::cout << "No solutions\n";
        } else {
            float sqr = std::sqrt((long double) ((race.first * race.first) - (4 * race.second)));
            uint64_t guess = std::round(sqr);
            if(guess * guess == (race.first * race.first) - (4 * race.second)) {
                uint64_t high = (race.first + guess - 1) / 2;
                uint64_t low = (race.first - guess + 2) / 2;
                //std::cout << "Integer solutions " << low << " to " << high << "\n";
                total *= (high - low + 1);
            } else {
                uint64_t high = (uint64_t) (((long double) race.first + sqr) / 2.0);
                uint64_t low = (uint64_t) (((long double) race.first - sqr + 2.0) / 2.0);           
                //std::cout << "Float solutions " << low << " to " << high << "\n";
                total *= (high - low + 1);
            }
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::vector<std::pair<uint64_t,uint64_t>> races;
    std::vector<std::pair<uint64_t,uint64_t>> one_race;

    std::string time, distance, wt, wd, tt, td;

    std::getline(input, time);
    std::getline(input, distance);

    std::stringstream st(time);
    std::stringstream sd(distance);

    while(getline(st, wt, ' ') && getline(sd, wd, ' ')) {
        while(wt == "") {
            getline(st, wt, ' ');
        }
        while(wd == "") {
            getline(sd, wd, ' ');
        }
        if(wt == "Time:" && wd == "Distance:") {
            continue;
        }
        tt = tt + wt;
        td = td + wd;
        races.emplace_back(std::stoull(wt), std::stoull(wd));
    }

    one_race.emplace_back(std::stoull(tt), std::stoull(td));

    std::cout << "Part 1: " << find_number_solutions(races) << "\n";
    std::cout << "Part 2: " << find_number_solutions(one_race) << "\n";
        
    return 0;
}