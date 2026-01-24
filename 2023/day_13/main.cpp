#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

int bitcount(int n) {
    int ret = 0;
    while(n) {
        n &= (n-1);
        ret++;
    }
    return ret;
}

int find_symmetry(std::vector<int>& nums, int flaws) {
    int ret = 0, step = 0;
    auto it = nums.begin();

    while(it+1 != nums.end()) {
        step++;
        int diff = 0;
        int match = true;
        auto fit = it+1;
        auto rit = it;

        while(fit != nums.end()) {
            if((diff += bitcount((*rit) ^ (*fit))) > flaws) {
                match = false;
            }
            if(rit == nums.begin()) {
                break;
            }
            rit--;
            fit++;
        }
        //std::cout << " match: " << match << " with " << diff << "/" << flaws << "flaws\n";
        if(match && diff == flaws) {
            ret = step;
            break;
        }

        it++;
    }

    return ret;
}

int count_symmetries(std::vector<std::vector<std::string>>& maps, int flaws = 0) {
    int total = 0;

    for(auto map : maps) {
        std::vector<int> rows;
        std::vector<int> cols = std::vector<int>(map[0].length());

        for(auto line : map) {
            int sum = 0;
            auto it = cols.begin();
            for(char c : line) {
                sum = (sum << 1) | (c == '#');
                *it = ((*it) << 1) | (c == '#');
                it++;
            }
            rows.push_back(sum);
        }

        total += 100 * find_symmetry(rows, flaws);
        total += find_symmetry(cols, flaws);
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::vector<std::string>> maps;
    bool more;

    do {
        std::vector<std::string> map;
        while((more = (bool) std::getline(input, line)) && line.length() > 0) {
            map.push_back(line);
        }
        maps.emplace_back(map);
    } while(more);

    std::cout << "Part1: " << count_symmetries(maps) << "\n";
    std::cout << "Part2: " << count_symmetries(maps, 1) << "\n";

    return 0;
}

// part2 - count the bits set in total if you xor both sides, look for points where the sum == 1
