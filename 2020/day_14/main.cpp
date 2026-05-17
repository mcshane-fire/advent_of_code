#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <format>
#include <set>

struct Writes {
    std::string mask;
    int64_t mask_0;
    int64_t mask_1;
    int64_t mask_x;
    std::vector<std::pair<int64_t,int64_t>> writes;

    Writes(std::string line) {
        mask = line.substr(7);
        mask_0 = (1LL<<36)-1;
        mask_1 = 0;
        mask_x = 0;
        int64_t sh = 1;
        for(auto it = line.rbegin(); it != line.rend() && *it != ' '; it++, sh<<=1) {
            switch(*it) {
                case '1': mask_1 |= sh; break;
                case '0': mask_0 &= ~sh; break;
                case 'X': mask_x |= sh; break;
            }
        }
    }

    void add(std::string line) {
        size_t r;
        int64_t addr = std::stoll(line.substr(4), &r);
        int64_t val = std::stoll(line.substr(8+r));
        writes.emplace_back(addr, val);
    }
};

int64_t sum_write_vals(std::vector<Writes> &program) {
    std::map<int64_t,int64_t> memory;
    for(auto &p : program) {
        for(auto &w : p.writes) {
            memory[w.first] = (w.second | p.mask_1) & p.mask_0;
        }
    }

    return std::accumulate(memory.begin(), memory.end(), 0LL, [](int64_t t, const auto &w) { return t + w.second; });
}

int64_t bitset(std::string val) {
    return 1LL << std::accumulate(val.begin(), val.end(), 0, [](int t, const char c) { return t + (c == 'X' ? 1 : 0); });
}

void add_all(std::string addr, int pos, std::set<std::string> &all) {
    if(pos == addr.length()) {
        all.insert(addr);
        return;
    }

    if(addr[pos] == 'X') {
        std::string n = addr;
        n[pos] = '0';
        add_all(n, pos+1, all);
        n[pos] = '1';
        add_all(n, pos+1, all);
    } else {
        add_all(addr, pos+1, all);
    }
}

void remove_writes(std::string memory, std::string write, int64_t val, std::map<std::string,int64_t> &left) {
    std::set<std::string> cur;
    std::set<std::string> remove;

    add_all(memory, 0, cur);
    add_all(write, 0, remove);   

    for(auto a : cur) {
        if(!remove.contains(a)) {
            left[a] = val;
        }
    }
}

int64_t sum_write_addrs(std::vector<Writes> &program) {
    std::map<std::string,int64_t> memory;

    for(auto &p : program) {

        for(auto &w : p.writes) {
            std::string addr = std::format("{:036b}", w.first);
            for(auto mit = p.mask.begin(), ait = addr.begin(); mit != p.mask.end(); mit++, ait++) {
                switch(*mit) {
                    case '1': *ait = '1'; break;
                    case 'X': *ait = 'X'; break;
                }
            }

            std::map<std::string,int64_t> new_memory;

            for(std::pair<std::string,int64_t> m : memory) {
                bool veto = false;
                for(auto mit = m.first.begin(), ait = addr.begin(); mit != m.first.end(); mit++, ait++) {
                    if((*mit == '1' && *ait == '0') || (*mit == '0' && *ait == '1')) {
                        veto = true;
                        break;
                    }
                }

                if(veto) {
                    new_memory[m.first] = m.second;
                } else {
                    remove_writes(m.first, addr, m.second, new_memory);
                }
            }

            new_memory[addr] = w.second;
            memory.clear();
            memory.swap(new_memory);
        }
    }

    return std::accumulate(memory.begin(), memory.end(), 0LL, [](int64_t t, const auto &w) { return t + (w.second * bitset(w.first)); });
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input2.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Writes> program;
    
    std::getline(input, line);
    while(line.length() > 0) {
        program.emplace_back(line);

        while(std::getline(input, line) && line[1] == 'e') {
            program.back().add(line);
        }
    }

    std::cout << "Part1: " << sum_write_vals(program) << "\n";
    std::cout << "Part2: " << sum_write_addrs(program) << "\n";

    return 0;
}
