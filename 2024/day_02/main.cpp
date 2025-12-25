#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

int is_valid(std::vector<int> seq, int skip) {
    int valid = 1;
    int change = 0;
    int first = 1;
    int prev;

    //std::cout << "is_valid skip: " << skip << "\n";

    for (size_t i = 0; i < seq.size(); i++) {
        if(i == skip) {
            continue;
        }

        int a = seq[i];
        if(first) {
            first = 0;
            prev = a;
        }
        else {
            if(prev == a || prev < a-3 || prev > a+3){
                valid = 0;
                break;
            }
            else if(change == 0) {
                change = a<prev ? -1 : 1;
            }
            else if((change < 0 && a>prev) || (change > 0 && a<prev)) {
                valid = 0;
                break;
            }
        }
        prev = a;
    }
    return valid;
}


int count_valid(std::vector<std::vector<int>> all, int escapes) {
    int total = 0;

    for (size_t i = 0; i < all.size(); i++) {

        if(escapes) {
            for(int j = 0; j<=all[i].size(); j++) {
                if(is_valid(all[i], j)) {
                    total += 1;
                    break;
                }
            }            
        } else {
            total += is_valid(all[i], -1);
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::vector<int>> all;

    while( std::getline( input, line ) ) {
        std::stringstream stream(line);
        std::vector<int> al;
        while (1) {
            int a;
            stream >> a;
            if (!stream) {
                break;
            }
            al.push_back(a);
        }
        all.push_back(al);
    }

    std::cout << "Part 1: " << count_valid(all, 0) << "\n";
    std::cout << "Part 2: " << count_valid(all, 1) << "\n";

    return 0;
}