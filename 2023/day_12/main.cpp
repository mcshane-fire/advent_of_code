#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

int64_t combinations(int blocks, int space) {
    if(blocks > space) {
        return 0;
    }
    int64_t res = 1;
    int64_t dmin = std::min(blocks, space-blocks);
    int64_t dmax = std::max(blocks, space-blocks);

    for(int64_t i=dmax+1; i<=space; i++) {
        res *= i;
    }
    for(int64_t i=1; i<=dmin; i++) {
        res /= i;
    }
    return res;
}

int64_t try_solution_2(std::string& inf, int pos, std::vector<int>& spec, int start) {
    int64_t total = 0;

    if(start == spec.size()) {
        for(int i=pos; i<inf.length(); i++) {
            if(inf[i] == '#') {
                //std::cout << " - solution leaves # unused at " << i << "\n";
                return 0;
            }
        }
        //std::cout << " completed, now " << total << "\n";
        return 1;
    }

    while(pos < inf.length()) {
        if(inf[pos] != '.') {
            int i=0;
            while(pos+i < inf.length() && inf[pos+i] == '?') {
                i++;
            }
            if(pos+i == inf.length() || inf[pos+i] == '.') {
                int j=0;
                int sum=0;
                while(start+j < spec.size()) {
                    sum += spec[start+j]+1;
                    j++;
                    if(sum <= i+1) {
                        //std::cout << " doing ? length " << i << " at pos " << pos << " with " << j << "blocks sum=" << sum << "\n";
                        //std::cout << " space is " << (i+1+j-sum) << " " << " for " << j << " -> " << combinations(j,i+1+j-sum) << "\n";
                        total += combinations(j,i+1+j-sum) * try_solution_2(inf, pos+i, spec, start+j);
                    } else {
                        break;
                    }
                }
                pos+=i;
            } else if(i >= 2 && inf[pos+i] == '#') {
                int j=0;
                int sum=0;
                int extra = 0;
                while(pos+i+extra+1 < inf.length() && inf[pos+i+extra+1] != '.') {
                    extra++;
                }
                //std::cout << " seen " << i << " ? at position " << pos << " followed by # (and " << extra << " following options)\n";
                while(start+j < spec.size() && sum <= i) {
                    // place block start+j on the # in all positions it can go
                    // only got 1+extra, so need to go back at least spec[start+j]-1-extra 
                    int min_offset = std::max(j == 0 ? 1 : 0,spec[start+j]-1-extra);
                    int max_offset = std::min(spec[start+j]-1, i-sum);
                    //std::cout << " prior " << " with " << j << " blocks, next block " << spec[start+j] << " between " << min_offset << " and " << max_offset << " before #\n";
                    for(int k=min_offset; k<=max_offset; k++) {
                        int next_pos = pos+i+spec[start+j]-k;
                        if(next_pos == inf.length() || inf[next_pos] != '#') {
                            if(next_pos != inf.length()) {
                                next_pos++;
                            }
                            int64_t result = combinations(j,i+j-sum-k) * try_solution_2(inf, next_pos, spec, start+j+1);
                            //std::cout << " placing at offset " << k << ", next " << next_pos << " got " << result << " solutions\n";
                            total += result;
                        }
                    }
                    sum += spec[start+j]+1;
                    j++;
                }
                pos += i;
            } else {
                int i=0;
                while(i < spec[start] && pos+i < inf.length() && (inf[pos+i] == '#' || inf[pos+i] == '?')) {
                    i++;
                }

                //std::cout << "start=" << start << " pos=" << pos << " i=" << i << "/" << spec[start] << "\n";
                //std::cout << "  pos+i=" << (pos+i) << " inf=" << inf.length() << " inf[pos+1]=" << inf[pos+i] << "\n";

                if(i == spec[start] && (pos+i == inf.length() || inf[pos+i] != '#')) {
                    //std::cout << " - recursing, spec[" << start << "]=" << spec[start] << " in position " << pos << "\n";
                    total += try_solution_2(inf, pos+i+1, spec, start+1);
                }
                
                if(inf[pos] == '#') {
                    //std::cout << " finishing, skipped over # at " << pos << "\n";
                    break;
                }
                pos++;
            }
        } else {
            if(inf[pos] == '#') {
                break;
            }
            pos++;
        }
    }

    return total;
}

int try_solution(std::string& inf, int pos, std::vector<int>& spec, int start) {
    int total = 0;

    if(start == spec.size()) {
        for(int i=pos; i<inf.length(); i++) {
            if(inf[i] == '#') {
                //std::cout << " - solution leaves # unused at " << i << "\n";
                return 0;
            }
        }

        //std::cout << " completed, now " << total << "\n";
        return 1;
    }

    while(pos < inf.length()) {
        if(inf[pos] != '.') {
            int i=0;
            while(i < spec[start] && pos+i < inf.length() && (inf[pos+i] == '#' || inf[pos+i] == '?')) {
                i++;
            }

            //std::cout << "start=" << start << " pos=" << pos << " i=" << i << "/" << spec[start] << "\n";
            //std::cout << "  pos+i=" << (pos+i) << " inf=" << inf.length() << " inf[pos+1]=" << inf[pos+i] << "\n";

            if(i == spec[start] && (pos+i == inf.length() || inf[pos+i] != '#')) {
                //std::cout << " - recursing, spec[" << start << "]=" << spec[start] << " in position " << pos << "\n";
                total += try_solution(inf, pos+i+1, spec, start+1);
            }
        } 
        
        if(inf[pos] == '#') {
            //std::cout << " finishing, skipped over # at " << pos << "\n";
            break;
        }
        pos++;
    }

    return total;
}

int64_t sum_solutions(std::vector<std::pair<std::string,std::vector<int>>>& problems, int copies = 1)
{
    int64_t total = 0;
    int64_t total2 = 0;

    for(int i=0; i<problems.size(); i++) {
        std::pair<std::string,std::vector<int>>& p = problems[i];
        std::string lp;
        std::vector<int> spec(p.second.size() * copies);
        lp += p.first;
        auto it = std::copy(p.second.begin(), p.second.end(), spec.begin());
        for(int i=1; i<copies; i++) {
            lp += "?";
            lp += p.first;
            it = std::copy(p.second.begin(), p.second.end(), it);
        }

        std::cout << "\nProblem " << i << ": " << lp << " ";
        for(auto i : spec) {
            std::cout << i << "-";
        }
        std::cout << "\n";

        //total += try_solution(lp, 0, spec, 0);
        total2 += try_solution_2(lp, 0, spec, 0);
        std::cout << "Now up to " << total << " " << total2 << "\n";
    }

    return total2;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::pair<std::string,std::vector<int>>> problems;

    while(std::getline(input, line)) {
        std::stringstream ss(line);
        std::string inf, seq, num;
        std::vector<int> nums;
        ss >> inf;
        ss >> seq;
        ss = std::stringstream(seq);
        while(getline(ss, num, ',')) {
            nums.push_back(std::stoi(num));
        }
        problems.emplace_back(inf, nums);
    }

    std::cout << "Part1: " << sum_solutions(problems) << "\n";
    std::cout << "Part2: " << sum_solutions(problems, 5) << "\n";

    return 0;
}
