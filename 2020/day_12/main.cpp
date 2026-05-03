#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int find_distance(std::vector<std::pair<char,int>> &instructions) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    int x = 0;
    int y = 0;
    int d = 0;

    for(auto &s : instructions) {
        switch(s.first) {
            case 'F': x += xd[d] * s.second; y += yd[d] * s.second; break;
            case 'N': y += s.second; break;
            case 'E': x += s.second; break;
            case 'S': y -= s.second; break;
            case 'W': x -= s.second; break;
            case 'R': d = (d + (s.second / 90)) % 4; break;
            case 'L': d = (d + 8 - (s.second / 90)) % 4; break;
        }
    }

    return std::abs(x) + std::abs(y);
}

void rotate(int &x, int &y, int steps) {
    switch(steps) {
        case 1: std::swap(x, y); y=-y; break;
        case 2: x=-x; y=-y; break;
        case 3: std::swap(x, y); x=-x; break;
    }
}

// 3256656 too high
// 7291 too low

int find_distance_waypoint(std::vector<std::pair<char,int>> &instructions, int wx, int wy) {
    int xd[] = {1, 0, -1, 0};
    int yd[] = {0, -1, 0, 1};

    int x = 0;
    int y = 0;

    for(auto &s : instructions) {
        switch(s.first) {
            case 'F': x += wx * s.second; y += wy * s.second; break;
            case 'N': wy += s.second; break;
            case 'E': wx += s.second; break;
            case 'S': wy -= s.second; break;
            case 'W': wx -= s.second; break;
            case 'R': rotate(wx, wy, (s.second / 90)); break;
            case 'L': rotate(wx, wy, 4-(s.second / 90)); break;
        }
    }

    return std::abs(x) + std::abs(y);
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<std::pair<char,int>> instructions;

    while(std::getline(input, line)) {
        instructions.emplace_back(line[0], std::stoi(line.substr(1)));
    }

    std::cout << "Part1: " << find_distance(instructions) << "\n";
    std::cout << "Part2: " << find_distance_waypoint(instructions, 10, 1) << "\n";

    return 0;
}
