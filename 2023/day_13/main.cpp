#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

int find_symmetry(std::vector<int>& nums) {
    int ret = 0, step = 0;

    //std::cout << "Find: ";

    auto it = nums.begin();

    while(it+1 != nums.end()) {
        step++;
        if(*it == *(it+1)) {
            //std::cout << "found double " << *it;
            bool match = true;

            auto fit = it+1;
            auto rit = it;
            while(fit != nums.end()) {
                if(*rit != *fit) {
                    match = false;
                }
                if(rit == nums.begin()) {
                    break;
                }
                rit--;
                fit++;
            }
            //std::cout << " match: " << match << "\n";
            if(match) {
                ret = step;
                break;
            }
        }
        it++;
    }

    /*
    if(ret == 0) {
        std::cout << "No symmetry: ";
        for(auto i : nums) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Symmetry(" << ret << "): ";
        for(int i=0; i<nums.size(); i++) {
            std::cout << nums[i] << (i+1 == ret ? "|" : " ");
        }
        std::cout << "\n";
    }*/

    return ret;
}

int count_symmetries(std::vector<std::vector<std::string>>& maps) {
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

        total += 100 * find_symmetry(rows);
        total += find_symmetry(cols);
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

    return 0;
}

// part2 - count the bits set in total if you xor both sides, look for points where the sum == 1
