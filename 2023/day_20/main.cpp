#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <sstream>
#include <numeric>

enum module_type {
    BROADCAST,
    FLIP_FLOP,
    CONJUNCTION,
};

struct module {
    enum module_type type;
    bool state;
    std::map<std::string,bool> inputs;
    std::vector<std::string> outputs;
};

struct pulse {
    std::string source;
    std::string target;
    bool high;
};

void send_pulse(std::map<std::string,struct module>& modules, std::vector<struct pulse>& next, struct pulse act) {
    //std::cout << "send_pulse: " << act.source << "  ---" << (act.high ? "high" : "low") << "---  " << act.target << "\n";
    if(!modules.contains(act.target)) {
        //std::cout << "Target " << act.target << " not found\n";
    } else {
        switch(modules[act.target].type) {
            case BROADCAST:
                for(auto t : modules[act.target].outputs) {
                    next.emplace_back(act.target, t, act.high);
                }
                break;
            case FLIP_FLOP:
                if(!act.high) {
                    modules[act.target].state = !modules[act.target].state;
                    for(auto t : modules[act.target].outputs) {
                        next.emplace_back(act.target, t, modules[act.target].state);
                    }
                }
                break;
            case CONJUNCTION:
                modules[act.target].inputs[act.source] = act.high;
                bool send = true;
                /*
                std::cout << "conj" << act.high << " ";
                for(auto p : modules[act.target].inputs) {
                    std::cout << p.first << "=" << p.second << " ";
                }
                std::cout << "\n";
                auto it = std::find_if(modules[act.target].inputs.begin(), modules[act.target].inputs.end(),
                    [&](const std::pair<std::string,bool>& p) { return p.second == false; });
                std::cout << " find_id = " << (*it).first << "\n";*/

                if(std::find_if(modules[act.target].inputs.begin(), modules[act.target].inputs.end(),
                    [&](const std::pair<std::string,bool>& p) { return p.second == false; }) == modules[act.target].inputs.end()) {
                    // no low signals found, so all are high, send low
                    //std::cout << act.target << " all are high, queueing low pulse\n";
                    send = false;
                //} else {
                //    std::cout << " found a low, queueing high pulse\n";
                }
                for(auto t : modules[act.target].outputs) {
                    next.emplace_back(act.target, t, send);
                }
        }
    }
}

int64_t count_pulses(std::map<std::string,struct module> modules, int pulses) {
    int64_t total_low = 0;
    int64_t total_high = 0;

    for(int i=0; i<pulses; i++) {
        // send low to broadcast
        std::vector<struct pulse> next;
        next.emplace_back("button", "broadcaster", false);
        while(next.size() > 0) {
            auto p = next.front();
            next.erase(next.begin());
            send_pulse(modules, next, p);
            if(p.high) {
                total_high++;
            } else {
                total_low++;
            }
        }
    }

    return total_high * total_low;
}

int64_t count_until_rx_one_low(std::map<std::string,struct module> modules) {

    std::vector<std::pair<std::string,std::vector<std::string>>> accu = {
        {"ns", {"br", "gv", "jl", "fp", "rb", "mf", "gp", "pb", "xv", "kh", "jz", "lj"}},
        {"hh", {"nc", "hj", "lk", "zd", "fm", "kq", "pp", "vs", "bp", "rv", "rk", "hr"}},
        {"kz", {"zn", "fv", "nh", "vx", "df", "gf", "zf", "qf", "tn", "lt", "ls", "hc"}},
        {"ck", {"tf", "tx", "mr", "rg", "tp", "pc", "vh", "tg", "cc", "rj", "sc", "vb"}}};
    std::set<std::string> track = {"lr", "gt", "nl", "vr"};
    std::vector<int64_t> cycles;

    std::map<enum module_type,char> tmap = {{FLIP_FLOP,'%'}, {CONJUNCTION,'&'}};
    int64_t ret = 0;
    while(ret < 4100) {
        // send low to broadcast
        ret++;

        std::vector<struct pulse> next;
        next.emplace_back("button", "broadcaster", false);
        while(next.size() > 0) {
            auto p = next.front();
            next.erase(next.begin());
            //std::cout << "send_pulse: "  << p.source << "  ---" << (p.high ? "high" : "low") << "---  " << p.target << "\n";

            send_pulse(modules, next, p);

            if(track.contains(p.target) && !p.high) {
                //std::cout << " low pulse to " << p.target << " after " << ret << "\n";
                cycles.push_back(ret);
            }
        }

        /*
        for(auto &p : accu) {
            std::cout << (modules[p.first].state ? "1" : "0") << ":";
            for(auto &s : p.second) {
                std::cout << (modules[s].state ? "1" : "0");
            }
            std::cout << "  ";
        }
        std::cout << "\n";
        */
    }


    int64_t total = 0;
    
    if(cycles.size() > 1) {
        total = std::lcm(cycles[0], cycles[1]);
        for(int i=2; i<cycles.size(); i++) {
            total = std::lcm(total, cycles[i]);
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input2.txt");
    std::string line;
    std::smatch res;
    std::regex module = std::regex("^([%&]*)([a-z]+) -> (.*)$");

    std::map<std::string,struct module> modules;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, module)) {
            struct module module;
            std::stringstream ss(res[3]);
            std::string ins;
            //std::cout << res[1] << ":";
            while(getline(ss, ins, ',')) {
                if(ins[0] == ' ') {
                    module.outputs.push_back(ins.substr(1));
                } else {
                    module.outputs.push_back(ins);
                }
            }
            if(res[1] == '&') {
                module.type = CONJUNCTION;
            } else if (res[1] == '%') {
                module.type = FLIP_FLOP;
            } else {
                module.type = BROADCAST;
            }
            module.state = false;
            modules[res[2]] = module;
        }
    }

    for(auto p : modules) {
        for(auto m : p.second.outputs) {
            if(modules[m].type == CONJUNCTION) {
                modules[m].inputs[p.first] = false;
            }
        }
    }

    std::cout << "Part1: " << count_pulses(modules, 1000) << "\n";
    std::cout << "Part2: " << count_until_rx_one_low(modules) << "\n";

    return 0;
}
