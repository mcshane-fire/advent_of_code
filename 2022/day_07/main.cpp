#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <regex>
#include <ranges>
#include <algorithm>

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::regex r_cd = std::regex("^\\$ cd (.*)$");
    std::regex r_file = std::regex("^(\\d+) (.*)$");
    std::smatch res;
    std::map<std::string,int> directories;
    std::string path;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, r_cd)) {
            if(res[1] == "/") {
                path = "/";
            } else if(res[1] == "..") {
                auto rit = std::find(path.rbegin()+1, path.rend(), '/');
                path.resize(path.rend() - rit);
            } else {
                path = path + std::string(res[1]) + "/";
            }
        } else if(std::regex_match(line, res, r_file)) {
            auto rit = path.rbegin();
            while(rit != path.rend()) {
                directories[path.substr(0, path.rend() - rit)] = std::stoi(res[1]) + directories[path.substr(0, path.rend() - rit)];
                rit = std::find(rit+1, path.rend(), '/');
            }
        }
    }

    std::cout << "Part1: " << std::accumulate(directories.begin(), directories.end(), 0, [](int a, auto d){return a + (d.second <= 100000 ? d.second : 0);}) << "\n";
    int free_up = directories["/"] - 40000000;
    std::cout << "Part2: " << std::ranges::min(directories | std::views::transform([](auto p) {return p.second; }) | std::views::filter([free_up](int p) {return p >= free_up; })) << "\n";

    return 0;
}