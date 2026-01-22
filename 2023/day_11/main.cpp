#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <numeric>

int64_t shortest_paths(std::vector<std::string>& map, int expand) {
    int64_t total = 0;

    std::vector<int64_t> hmap(map[0].length(),expand-1);
    std::vector<int64_t> vmap(map.size());
    std::vector<std::pair<int,int>> stars;

    int64_t extra = 0;
    for(int i=0; i<map.size(); i++) {
        size_t p = map[i].find('#');
        if(p == std::string::npos) {
            extra += expand-1;
        } else {
            do {
                hmap[p] = 0;
                stars.emplace_back(i, p);
            } while((p = map[i].find('#',p+1)) != std::string::npos);
        }
        vmap[i] = i + extra;
    }

    extra = 0;
    for(int i=0; i<hmap.size(); i++) {
        extra += hmap[i];
        hmap[i] = i + extra;
    }

    for(int i=0; i<stars.size(); i++) {
        for(int j=i+1; j<stars.size(); j++) {
            total += std::abs(vmap[stars[i].first] - vmap[stars[j].first]) + std::abs(hmap[stars[i].second] - hmap[stars[j].second]);
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::string> map;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::cout << "Part1: " << shortest_paths(map, 2) << "\n";
    std::cout << "Part2: " << shortest_paths(map, 1000000) << "\n";    
    return 0;
}
