#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>

struct Meal {
    std::vector<std::string> ingredients;
    int danger;

    Meal(std::string line, std::vector<std::string> &allergens) {
        bool into_danger = false;
        while(line.length() > 0) {
            if(line[0] == '(') {
                into_danger = true;
                line = line.substr(10);
            }

            auto it = std::find_if(line.begin(), line.end(), [](const char c) { return c == ' ' || c == ',' || c == ')';});
            std::string word = line.substr(0, std::distance(line.begin(), it));
            if(into_danger) {
                auto ait = std::find(allergens.begin(), allergens.end(), word);
                if(ait == allergens.end()) {
                    danger |= 1 << allergens.size();
                    allergens.push_back(word);
                } else {
                    danger |= 1 << std::distance(allergens.begin(), ait);
                }
            } else {
                ingredients.push_back(word);
            }

            line = line.substr(word.length() + (*it == ',' ? 2 : 1));
        }
    }
};

int bitcount(int num) {
    int count = 0;
    while(num != 0) {
        num &= (num-1);
        count++;
    }
    return count;
}

int find_non_toxic(std::vector<Meal> &meals, std::vector<std::string> &allergens) {
    std::map<std::string,std::set<int>> map;
    std::map<std::string,int> possible;
    int ret = 0;
    
    for(int i=0; i<meals.size(); i++) {
        for(auto &ing : meals[i].ingredients) {
            map[ing].insert(i);
        }
    }

    for(auto &v : map) {
        int all_possible = 0;
        for(int m : v.second) {
            int candidate = meals[m].danger;
            for(int ni=0; ni<meals.size(); ni++) {
                if(!v.second.contains(ni)) {
                    candidate &= ~meals[ni].danger;
                }
            }
            all_possible |= candidate;
        }
        
        if(all_possible == 0) {
            ret += v.second.size();
        } else {
            possible[v.first] = all_possible;
        }
    }

    // work out which ingredient has which allergen, posible part 2 solution
    bool progress = true;
    while(progress) {
        progress = false;
        for(auto &p : possible) {
            if(bitcount(p.second) == 1) {
                for(auto &p2 : possible) {
                    if(p.first != p2.first && (p2.second & p.second) != 0) {
                        p2.second &= ~p.second;
                        progress = true;
                    }
                }
            }
        }
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Meal> meals;
    std::vector<std::string> allergens;


    while(std::getline(input, line)) {
        meals.emplace_back(line, allergens);
    }

    std::cout << "Part1: " << find_non_toxic(meals, allergens) << "\n";

    return 0;
}
