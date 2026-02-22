#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

int follow_tail(std::string filename, int length) {
    std::ifstream input(filename);
    std::string line;
    std::set<std::pair<int,int>> visited;

    std::map<char,int> xdir = {{'L',-1}, {'R',1}};
    std::map<char,int> ydir = {{'U',-1}, {'D',1}};
    std::vector<std::pair<int,int>> rope(length);

    while(std::getline(input, line)) {
        for(int i=0; i<std::stoi(line.substr(2)); i++) {
            rope[0].first += xdir[line[0]];
            rope[0].second += ydir[line[0]];

            auto it = rope.begin();
            auto nit = std::next(rope.begin());
            while(nit != rope.end()) {
                int fd = std::abs((*it).first - (*nit).first);
                int sd = std::abs((*it).second - (*nit).second);

                if(fd == 2) {
                    (*nit).first = ((*it).first + (*nit).first) >> 1;
                    if(sd == 1) {
                        (*nit).second = (*it).second;
                    } else if(sd == 2) {
                        (*nit).second = ((*it).second + (*nit).second) >> 1;
                    }
                } else if(sd == 2) {
                    (*nit).second = ((*it).second + (*nit).second) >> 1;
                    if(fd == 1) {
                        (*nit).first = (*it).first;
                    }
                }
                it = nit;
                nit++;
            }

            visited.insert(*it);
        }
    }

    return visited.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";

    std::cout << "Part1: " << follow_tail(filename, 2) << "\n";
    std::cout << "Part2: " << follow_tail(filename, 10) << "\n";

    return 0;
}