#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <set>

int count_match(std::vector<std::map<std::string,std::string>> &passports, std::vector<std::string> req) {
    int num = 0;
    for(auto &p : passports) {
        num++;
        for(auto &r : req) {
            if(!p.contains(r)) {
                num--;
                break;
            }
        }
    }

    return num;
}

int count_valid(std::vector<std::map<std::string,std::string>> &passports, std::vector<std::string> req) {
    int num = 0;
    std::regex year = std::regex("^\\d\\d\\d\\d$");
    std::map<std::string,std::pair<int,int>> year_limits = {{"byr", {1920,2002}}, {"iyr", {2010, 2020}}, {"eyr", {2020, 2030}}};
    std::regex height = std::regex("^(\\d+)(in|cm)$");
    std::map<std::string,std::pair<int,int>> height_limits = {{"in", {59, 76}}, {"cm", {150,193}}};
    std::regex hair = std::regex("^#([0-9a-f]*)$");
    std::set<std::string> eye = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    std::regex number = std::regex("^\\d\\d\\d\\d\\d\\d\\d\\d\\d$");
    std::smatch res;

    for(auto &p : passports) {
        bool valid = true;
        for(auto &r : req) {
            if(!p.contains(r)) {
                valid = false;
            } else {
                if(r == "byr" || r == "iyr" || r == "eyr") {
                    if(std::regex_match(p[r], res, year)) {
                        int y = std::stoi(p[r]);
                        if(y < year_limits[r].first || y > year_limits[r].second) {
                            valid = false;
                        }
                    } else {
                        valid = false;
                    }
                }
                else if(r == "hgt") {
                    if(std::regex_match(p[r], res, height)) {
                        int h = std::stoi(res[1]);
                        if(h < height_limits[res[2]].first || h > height_limits[res[2]].second) {
                            valid = false;
                        }
                    } else {
                        valid = false;
                    }
                } else if(r == "hcl") {
                    if(!std::regex_match(p[r], res, hair) || res[1].length() != 6) {
                        valid = false;
                    }
                } else if(r == "ecl") {
                    if(!eye.contains(p[r])) {
                        valid = false;
                    }
                } else if(r == "pid") {
                    if(!std::regex_match(p[r], res, number)) {
                        valid = false;
                    }
                }
            }
        }
        if(valid) {
            num++;
        }
    }

    return num;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_valid.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<std::map<std::string,std::string>> passports;
    std::regex pat = std::regex("^([a-z]+):([^ ]*)");
    std::smatch res;

    while(std::getline(input, line)) {
        if(passports.size() == 0 || line.length() == 0){
            passports.push_back(std::map<std::string,std::string>());
        }

        while(std::regex_search(line, res, pat)) {
            passports.back()[res[1]] = res[2];
            auto it = std::find(line.begin(), line.end(), ' ');
            if(it == line.end()) {
                break;
            }
            line = line.substr(std::distance(line.begin(), it)+1);
        }
    }

    std::cout << "Part1: " << count_match(passports, {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}) << "\n";
    std::cout << "Part2: " << count_valid(passports, {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}) << "\n";

    return 0;
}
