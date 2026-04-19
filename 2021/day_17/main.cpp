#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <climits>
#include <set>

int find_min_x(int x[]) {
    int min_x = 0;
    while(true) {
        min_x++;
        int d = ((min_x * min_x) + min_x) / 2;
        if(d > x[0]) {
            break;
        }
    }
    return min_x;
}

void find_steps(std::vector<int> &steps, int x[], int x_speed, int max_steps) {
    int xp = 0;
    int count = 0;
    while(xp <= x[1] && count < max_steps) {
        count++;
        xp += x_speed;
        if(x_speed > 0) {
            x_speed--;
        }
        if(xp >= x[0] && xp <= x[1]) {
            steps.push_back(count);
        }
    }
}

int highest_trajectory(int x[], int y[]) {
    int max_y = -1-y[0];
    return ((max_y * max_y) + max_y)/2;
}

int count_possible(int x[], int y[]) {
    int x_speed = find_min_x(x);
    int ret = 0;
    std::set<std::pair<int,int>> speeds;

    while(x_speed <= x[1]) {
        std::vector<int> steps;
        find_steps(steps, x, x_speed, 2-(2*y[0]));

        for(int s : steps) {
            if(s == INT_MAX) {
                break;
            }
            for(int y_speed = y[0]; y_speed < -y[0]; y_speed++) {
                int y_pos;
                if(y_speed > 0) {
                    if(s <= (y_speed*2)+1) {
                        continue;
                    }
                    int height = ((y_speed * y_speed) + y_speed) / 2;
                    int sr = s - (y_speed+1);
                    y_pos = height - (((sr * sr) + sr) / 2);
                } else {
                    y_pos = (y_speed * s) - ((((s-1)*(s-1))+s-1)/2);
                }
                if(y_pos >= y[0] && y_pos <= y[1]) {
                    speeds.insert({x_speed,y_speed});
                }
            }
        }

        x_speed++;
    }

    return speeds.size();
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex pat = std::regex("^target area: x=(\\d+)..(\\d+), y=(-\\d+)..(-\\d+)$");
    std::smatch res;
    int x[2];
    int y[2];

    std::getline(input, line);
    if(std::regex_match(line, res, pat)) {
        x[0] = std::stoi(res[1]);
        x[1] = std::stoi(res[2]);
        y[0] = std::stoi(res[3]);
        y[1] = std::stoi(res[4]);
    }

    std::cout << "Part1: " << highest_trajectory(x, y) << "\n";
    std::cout << "Part2: " << count_possible(x, y) << "\n";

    return 0;
}
