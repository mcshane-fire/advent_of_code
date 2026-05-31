#include <string>
#include <fstream>
#include <iostream>
#include <map>

struct Sequence {
    std::map<int,int> seen;
    int turn;
    int next;

    Sequence(std::string line) {
        turn = 0;
        while(line.length() > 0) {
            size_t pos;
            add(std::stoi(line, &pos));
            line = line.substr(std::min(line.length(), pos+1));
        }
    }

    void add(int num) {
        next = seen.contains(num) ? turn - seen[num] : 0;
        seen[num] = turn++;
    }

    int get_next() {
        int ret = next;
        add(next);
        return ret;
    }
};

int iterate(Sequence seq, int target) {
    int ret;
    while(seq.turn < target) {
        ret = seq.get_next();
    }

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
        
    std::getline(input, line);
    Sequence seq(line);

    std::cout << "Part1: " << iterate(seq, 2020) << "\n";
    std::cout << "Part2: " << iterate(seq, 30000000) << "\n";
    return 0;
}
