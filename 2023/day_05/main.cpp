#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <numeric>
#include <regex>

struct map_range {
    uint64_t dest;
    uint64_t source;
    uint64_t length;

    friend std::ostream& operator<< (std::ostream& stream, const struct map_range &t) {
        stream << "(" << t.source << "-" << (t.source+t.length-1) << " -> " << t.dest << "-" << (t.dest+t.length-1) << ")";
        return stream;
    }

};

uint64_t find_lowest_location(std::vector<uint64_t>& seeds, std::vector<std::vector<struct map_range>>& maps) {
    int lowest;
    bool set = false;

    for(uint64_t num : seeds) {
        for(auto map : maps) {
            for(auto mr : map) {
                if(num >= mr.source && num < mr.source+mr.length) {
                    num = num - mr.source + mr.dest;
                    break;
                }
            }
        }
        if(!set || num < lowest) {
            lowest = num;
            set = true;
        }
    }

    return lowest;
}

uint64_t find_lowest_mapping_ranges(std::vector<uint64_t>& seeds, std::vector<std::vector<struct map_range>>& maps) {
    std::vector<std::pair<uint64_t,uint64_t>> ranges;
    for(uint i=0; i<seeds.size(); i+=2) {
        ranges.emplace_back(seeds[i], seeds[i+1]);
    }

    for(auto map_set : maps) {
        //std::cout << "map_ranges with these maps:\n";

        std::sort(map_set.begin(), map_set.end(), [](const struct map_range& a, const struct map_range& b) { return a.source < b.source; });
        std::vector<std::pair<uint64_t,uint64_t>> next_ranges = std::vector<std::pair<uint64_t,uint64_t>>();

        for(auto& range : ranges) {
            //std::cout << "range:  " << range.first << " to " << (range.first+range.second-1) << "\n";
            for(auto map : map_set) {
                if(range.second == 0) {
                    //std::cout << " -exhausted range, breaking\n";
                    break;
                }
                //std::cout << " -range now " << range.first << " to " << (range.first+range.second-1) << ", apply map " << map << "\n";
                if(map.source+map.length <= range.first) {
                    //std::cout << "   ignoring " << map << " as before range\n";
                } else if(map.source >= range.first+range.second) {
                    //std::cout << "   ignoring " << map << " as after range\n";
                    if(range.second != 0) {
                        next_ranges.emplace_back(range.first, range.second);
                    }
                } else if(map.source < range.first && map.source + map.length < range.first+range.second) {
                    uint64_t eaten = map.source + map.length - range.first;
                    //std::cout << "   map " << map << " eats " << eaten << " from the start\n";
                    next_ranges.emplace_back(range.first + (map.dest - map.source), eaten);
                    range.first += eaten;
                    range.second -= eaten;
                } else if(map.source < range.first) {
                    //std::cout << "   map " << map << " entirely covers range\n";
                    next_ranges.emplace_back(range.first + (map.dest - map.source), range.second);
                    range.second = 0;
                } else if (map.source + map.length >= range.first + range.second) {
                    uint64_t eaten = range.first + range.second - map.source;
                    //std::cout << "   map " << map << " eats " << eaten << " from the end\n";
                    next_ranges.emplace_back(range.first, range.second - eaten);
                    next_ranges.emplace_back(range.first + range.second - eaten + (map.dest - map.source), eaten);
                    range.second = 0;
                } else {
                    //std::cout << "   map " << map << " entirely within range";
                    next_ranges.emplace_back(range.first, map.source - range.first);
                    next_ranges.emplace_back(map.dest, map.length);
                    range.second -= map.length + map.source - range.first;
                    range.first = map.source + map.length;
                }
            }

            if(range.second > 0) {
                next_ranges.emplace_back(range.first, range.second);
            }
        }

        ranges.clear();
        for(auto range : next_ranges) {
            if(range.second > 0) {
                ranges.push_back(range);
            }
        }
    }

    std::sort(ranges.begin(), ranges.end(), [](const std::pair<uint64_t,uint64_t>& a, const std::pair<uint64_t,uint64_t>& b) { return a.first < b.first; });
    return ranges[0].first;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::regex triple = std::regex("^(\\d+) (\\d+) (\\d+)$");
    std::smatch res;

    std::vector<uint64_t> seeds;
    std::vector<std::vector<struct map_range>> maps;

    while(std::getline(input, line)) { 
        std::stringstream ss(line);
        std::string word;
        getline(ss, word, ' ');
        if(word == "seeds:") {
            while(getline(ss, word, ' ')) {
                seeds.push_back(std::stoull(word));
            }
            std::getline(input, line);
        } else {
            std::vector<struct map_range> map;
            while(std::getline(input, line)) {
                if(line.length() == 0) {
                    break;
                }
                if(std::regex_match(line, res, triple)) {
                    struct map_range t(std::stoull(res[1]), std::stoull(res[2]), std::stoull(res[3]));
                    map.push_back(t);
                }
            }
            maps.push_back(map);
        }
    }

    std::cout << "Part 1: " << find_lowest_location(seeds, maps) << "\n";
    std::cout << "Part 2: " << find_lowest_mapping_ranges(seeds, maps) << "\n";
        
    return 0;
}