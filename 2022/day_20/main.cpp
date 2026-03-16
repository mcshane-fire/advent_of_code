#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int64_t decrypt(std::vector<int64_t> v, int64_t decryption_key = 1, int rounds = 1) {
    std::vector<int> index(v.size());
    std::iota(index.begin(), index.end(), 0);

    for(int r=0; r<rounds; r++) {
        for(int i=0; i<index.size(); i++) {
            int s = index[i];
            int64_t val = (v[s] * decryption_key) % (int64_t) (index.size() - 1);
            int d = val + s;

            if(val < 0 && val + s <= 0) {
                // going backwards & wrapping
                d += index.size()-1;
            } else if(val > 0 && val + s >= index.size()) {
                // going forwards & wrapping
                d -= index.size()-1;
            }

            auto di = v.begin();
            std::advance(di, d);
            auto ci = v.begin();
            std::advance(ci, s);

            if(d < index[i]) { 
                std::rotate(di, ci, ci+1);
                std::transform(index.begin(), index.end(), index.begin(), [s, d](int n) { return n >= d && n < s ? n+1 : n; });
            } else if(index[i] < d) {
                std::rotate(ci, ci+1, di+1);
                std::transform(index.begin(), index.end(), index.begin(), [s, d](int n) { return n > s && n <= d ? n-1 : n; });
            }

            index[i] = d;
        }
    }

    int id = std::find(v.begin(), v.end(), 0) - v.begin();
    return (v[(id + 1000) % v.size()] + v[(id + 2000) % v.size()] + v[(id + 3000) % v.size()]) * decryption_key;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<int64_t> file;

    while(std::getline(input, line)) {
        file.push_back(std::stoi(line));
    }

    std::cout << "Part1: " << decrypt(file) << "\n";
    std::cout << "Part2: " << decrypt(file, 811589153, 10) << "\n";

    return 0;
}
