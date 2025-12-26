#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <set>

int count_middle_pages(std::vector<std::vector<int>> sequences, std::unordered_map<int, std::vector<int>> rules, int include_valid) {
    int total = 0;

    for(auto seq : sequences) {
        //std::cout << "Sequence: " << seq.size() << "\n";
        std::set<int> invalid;
        int valid = 1;

        for(int a : seq) {
            //std::cout << a << " ";

            if(invalid.contains(a)) {
                //std::cout << "Invalid " << a << "\n";
                valid = 0;
                break;
            }

            if(rules.contains(a)) {
                invalid.insert(rules[a].begin(), rules[a].end());
            }
        }
        //std::cout << "\nValid: " << valid << "\n";

        if(include_valid) {
            if(valid) {
                total += seq[(seq.size() -1) / 2];
            }
        } else {
            if(!valid) {
                std::vector<int> corrected;
                std::unordered_map<int, int> children;
                for(int a : seq) {
                    children[a] = 0;
                }
                for(int a : seq) {
                    if(rules.contains(a)) {
                        for(int b : rules[a]) {
                            if(children.contains(b)) {
                                children[b]++;
                            }
                        }
                    }
                }
                int next = -1;
                for (int a : seq) {
                    if(children[a] == 0) {
                        next = a;
                    }
                }
                while(next != -1) {
                    corrected.push_back(next);
                    //std::cout << "Next element: " << next << "\n";
                    int found = -1;
                    for(int a : rules[next]) {
                        if(children.contains(a)) {
                            children[a]--;
                            if(children[a] == 0) {
                                found = a;
                            }
                        }
                    }
                    next = found;
                }
                total += corrected[(corrected.size()-1) / 2];
            }
        }
    }

    return total;
}


int main(int argc, char *argv[]) {
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::string> lines;
    std::regex rule("^(\\d+)\\|(\\d+)$");
    std::regex num("\\d+");
    std::smatch res;
    std::unordered_map<int, std::vector<int>> rules;

    std::vector<std::vector<int>> sequences;

    while(std::getline(input, line)) {
        //std::cout << "Read "<<line<<"\n";
        if(std::regex_match(line, res, rule)) {
            int n1 = stoi(res[1]);
            int n2 = stoi(res[2]);
            //std::cout << "rule match "<< n1 <<", "<< n2 <<"\n";
            if(!rules.contains(n2)) {
                std::vector<int> nv;
                rules[n2] = nv = {n1};
            } else {
                rules[n2].push_back(n1);
            }

        } else if (line.length() > 0) {
            std::string::const_iterator start(line.cbegin());
            std::vector<int> all;

            while(std::regex_search(start, line.cend(), res, num)) {
                all.push_back(stoi(res[0]));
                start = res.suffix().first;
            }
            sequences.push_back(all);
        }
        lines.push_back(line);
    }

    std::cout << "Part 1: " << count_middle_pages(sequences, rules, 1) << "\n";
    std::cout << "Part 2: " << count_middle_pages(sequences, rules, 0) << "\n";

    return 0;
}