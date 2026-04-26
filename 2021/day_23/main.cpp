#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

struct Layout {
    int hallway[11] = {0,0,0,0,0,0,0,0,0,0,0};
    std::vector<int> room[4];

    auto operator<=>(const Layout &lhs) const = default;
};

int sort_rooms(Layout &lay) {

    std::map<Layout,int> best;
    std::map<int,std::vector<Layout>> exp = {{0, {lay}}};
    std::map<int,int> cost = {{1,1}, {2,10}, {3,100}, {4,1000}};

    while(exp.size() > 0) {
        auto p = exp.extract(exp.begin());

        for(auto &a : p.mapped()) {

            if(!best.contains(a)) {
                best[a] = p.key();

                int check = 0;
                while(check < 4 && std::find_if(a.room[check].begin(), a.room[check].end(), [check](const int a) {return a != check+1; }) == a.room[check].end()) {
                    check++;
                }
                if(check == 4) {
                    return p.key();
                }

                // moving out of rooms
                for(int i=0; i<4; i++) {
                    int pos = 2 + (i*2);
                    for(int s=0; s<a.room[i].size(); s++) {
                        if(a.room[i][s] == 0) {
                            continue;
                        } else {
                            int d=pos-1;
                            while(d>=0 && a.hallway[d] == 0) {
                                if((d&1) || d==0 || d==10) {
                                    Layout n = a;
                                    std::swap(n.hallway[d], n.room[i][s]);
                                    exp[p.key() + (cost[n.hallway[d]] * ((pos-d) + s + 1))].push_back(n);
                                }
                                d--;
                            }
                            d=pos+1;
                            while(d<11 && a.hallway[d] == 0) {
                                if((d&1) || d == 0 || d==10) {
                                    Layout n = a;
                                    std::swap(n.hallway[d], n.room[i][s]);
                                    exp[p.key() + (cost[n.hallway[d]] * ((d-pos) + s + 1))].push_back(n);
                                }
                                d++;
                            }
                            break;
                        }
                    }
                }

                // moving out of hallway
                for(int pos=0; pos<11; pos++) {
                    if(a.hallway[pos] != 0) {
                        int t = a.hallway[pos];
                        if(std::find_if(a.room[t-1].begin(), a.room[t-1].end(), [t](const int n) { return n != 0 && n != t; }) == a.room[t-1].end()) {
                            int target = t * 2;
                            int diff = target > pos ? 1 : -1;
                            int cur = pos+diff;
                            while(cur != target+diff) {
                                if(a.hallway[cur] != 0) {
                                    break;
                                }
                                cur += diff;
                            }
                            if(cur == target+diff) {
                                int h = a.room[t-1].rend() - std::find(a.room[t-1].rbegin(), a.room[t-1].rend(), 0) - 1;
                                Layout n = a;
                                std::swap(n.hallway[pos], n.room[t-1][h]);
                                exp[p.key() + (cost[t] * (((target - pos) * diff) + h + 1))].push_back(n);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    int extra[2][4] = {{4, 2, 1, 3}, {4, 3, 2, 1}};

    Layout lay;

    while(std::getline(input, line)) {
        if(line[2] == '#' && line[3] != '#') {
            for(int i=0; i<4; i++) {
                lay.room[i].push_back(line[3+(2*i)]-'A'+1);
            }
        }
    }
    
    std::cout << "Part1: " << sort_rooms(lay) << "\n";

    for(int i=0; i<2; i++) {
        for(int j=0; j<4; j++) {
            lay.room[j].insert(std::next(lay.room[j].begin()), extra[i][j]);
        }
    }

    std::cout << "Part2: " << sort_rooms(lay) << "\n";

    return 0;
}
