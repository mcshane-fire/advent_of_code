#include <string>
#include <fstream>
#include <iostream>
#include <vector>

enum ItemType {
    OPEN_BRACKET,
    COMMA,
    CLOSE_BRACKET,
    NUMBER
};

class Snailfish {
    public:

    Snailfish(std::string line) {
        for(auto it = line.begin(); it != line.end(); it++) {
            if(*it == '[') {
                data.emplace_back(OPEN_BRACKET,0);
            } else if (*it == ']') {
                data.emplace_back(CLOSE_BRACKET,0);
            } else if (*it == ',') {
                data.emplace_back(COMMA,0);
            } else if (*it >= '0' && *it <= '9') {
                data.emplace_back(NUMBER,(*it)-'0');
            }
        }
    }

    Snailfish() {}

    Snailfish operator+(const Snailfish& s) {
        Snailfish n;
        n.data.emplace_back(OPEN_BRACKET,0);
        n.data.insert(n.data.end(), data.begin(), data.end());
        n.data.emplace_back(COMMA,0);
        n.data.insert(n.data.end(), s.data.begin(), s.data.end());
        n.data.emplace_back(CLOSE_BRACKET,0);
        n.reduce();
        return n;
    }

    int magnitude() {
        int total = 0;
        int mul = 1;
        for(int i=0; i<data.size(); i++) {
            if(data[i].first == OPEN_BRACKET) {
                mul *= 3;
            } else if(data[i].first == COMMA) {
                mul /= 3;
                mul *= 2;
            } else if(data[i].first == CLOSE_BRACKET) {
                mul /= 2;
            } else {
                total += mul * data[i].second;
            }
        }
        return total;
    }

    friend std::ostream& operator<<(std::ostream &os, const Snailfish &s) {
        for(auto &p : s.data) {
            if(p.first == OPEN_BRACKET) {
                os << "[";
            } else if(p.first == CLOSE_BRACKET) {
                os << "]";
            } else if(p.first == COMMA) {
                os << ",";
            } else if(p.first == NUMBER) {
                os << p.second;
            }
        }
        return os;
    }

    private:

    void reduce() {
        bool again = true;
        while(again) {
            while(again) {
                again = false;
                int depth = 0;
                for(int i=0; i<data.size(); i++) {
                    if(data[i].first == OPEN_BRACKET) {
                        depth++;
                        if(depth == 5) {
                            again = true;
                            for(int j=i-1; j>=0; j--) {
                                if(data[j].first == NUMBER) {
                                    data[j].second += data[i+1].second;
                                    break;
                                }
                            }
                            for(int j=i+5; j<data.size(); j++) {
                                if(data[j].first == NUMBER) {
                                    data[j].second += data[i+3].second;
                                    break;
                                }
                            }
                            data[i].first = NUMBER;
                            data[i].second = 0;
                            data.erase(data.begin()+i+1, data.begin()+i+5);
                            break;
                        }
                    } else if(data[i].first == CLOSE_BRACKET) {
                        depth--;
                    }
                }            
            }

            for(int i=0; i<data.size(); i++) {
                if(data[i].first == NUMBER && data[i].second > 9) {
                    again = true;
                    data.insert(data.begin()+i+1, {NUMBER, data[i].second/2});
                    data.insert(data.begin()+i+2, {COMMA, 0});
                    data.insert(data.begin()+i+3, {NUMBER, (data[i].second+1)/2});
                    data.insert(data.begin()+i+4, {CLOSE_BRACKET, 0});
                    data[i] = {OPEN_BRACKET,0};
                    break;
                }
            }
        }
    }

    std::vector<std::pair<ItemType,int>> data;

};

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Snailfish> fishes;

    while(std::getline(input, line)) {
        fishes.emplace_back(Snailfish(line));
    }

    Snailfish total = fishes[0] + fishes[1];
    for(int i=2; i<fishes.size(); i++) {
        total = total + fishes[i];
    }

    std::cout << "Part1: " << total.magnitude() << "\n";

    int best = 0;
    for(int i=0; i<fishes.size(); i++) {
        for(int j=0; j<fishes.size(); j++) {
            best = std::max(best, (fishes[i] + fishes[j]).magnitude());
        }
    }

    std::cout << "Part2: " << best << "\n";

    return 0;
}
