#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int moves(std::vector<int> ring, int num) {
    int min_label = *std::min_element(ring.begin(), ring.end());
    int max_label = *std::max_element(ring.begin(), ring.end());
    for(int i=0; i<num; i++) {
        std::vector<int> next(ring.size());
        int dest = ring[0] == min_label ? max_label : ring[0]-1;
        auto it = std::find(ring.begin(), ring.end(), dest);
        while(std::distance(ring.begin(), it) < 4) {
            dest = dest == min_label ? max_label : dest-1;
            it = std::find(ring.begin(), ring.end(), dest);
        }
       
        auto dit = std::copy(std::next(ring.begin(), 4), std::next(it), next.begin());
        dit = std::copy(std::next(ring.begin()), std::next(ring.begin(),4), dit);
        dit = std::copy(std::next(it), ring.end(), dit);
        *dit = ring[0];
        std::copy(next.begin(), next.end(), ring.begin());
    }

    auto it = std::find(ring.begin(), ring.end(), 1);
    int ret = 0;
    while(true) {
        it++;
        if(it == ring.end()) {
            it = ring.begin();
        }
        if(*it == 1) {
            break;
        }
        ret = (ret * 10) + *it;
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int> ring;

    std::getline(input, line);
    for(char c : line) {
        ring.push_back(c - '0');
    }

    std::cout << "Part1: " << moves(ring, 100) << "\n";

    return 0;
}
