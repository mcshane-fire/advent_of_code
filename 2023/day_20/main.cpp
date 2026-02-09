#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <sstream>

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
                    //std::cout << " all are high, queueing low pulse\n";
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

int64_t count_pulses(std::map<std::string,struct module>& modules, int pulses) {
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

int64_t count_until_rx_one_low(std::map<std::string,struct module>& modules) {

    int num = 0;
    for(auto& p : modules) {
        if(p.second.type == CONJUNCTION) {
            num += modules[p.first].inputs.size();
        }
    }

    std::vector<std::map<int,int>> history(num, std::map<int,int>());
    std::vector<std::pair<bool,int>> runs(num, std::pair<bool,int>());
    std::map<enum module_type,char> tmap = {{FLIP_FLOP,'%'}, {CONJUNCTION,'&'}};
    int64_t ret = 0;
    while(ret < 300000) {
        // send low to broadcast
        ret++;
        if(true) {
            int num = 0;
            for(auto& p : modules) {
                if(p.second.type == CONJUNCTION) {
                    //std::cout << p.first << "(";
                    for(auto& inp : modules[p.first].inputs) {
                        //std::cout << inp.second;
                        if(runs[num].second == 0) {
                            runs[num].first = inp.second;
                            runs[num].second = 1;
                        } else if(runs[num].first != inp.second) {
                            if(!history[num].contains(runs[num].second)) {
                                history[num][runs[num].second] = 0;
                            } else {
                                history[num][runs[num].second]++;
                            }
                            runs[num].first = inp.second;
                            runs[num].second = 1;
                        } else {
                            runs[num].second++;
                        }
                        num++;
                    }
                    //std::cout << ")";
                }
            }
            //std::cout << "\n";
            //std::cout << ret << " " << num << " " << std::format("{:038b}", state) << "\n";
        }
        std::vector<struct pulse> next;
        next.emplace_back("button", "broadcaster", false);
        while(next.size() > 0) {
            auto p = next.front();
            next.erase(next.begin());
            //std::cout << "send_pulse: "  << p.source << "  ---" << (p.high ? "high" : "low") << "---  " << p.target << "\n";

            send_pulse(modules, next, p);
            if(p.target == "rx" && !p.high) {
                std::cout << " low pulse to rx " << "\n";
                break;
            }
        }
    }

    for(auto& h : history) {
        for(auto& p : h) {
            if(p.second > 0) {
                std::cout << p.first << "," << p.second << "  ";
            }
        }
        std::cout << "\n";
    }

    return ret;
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


