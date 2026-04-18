#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <climits>

class BitStream {
    public:
    BitStream(std::string line) {
        for(auto it = line.begin(); it != line.end(); it++) {
            if(*it >= '0' && *it <= '9') {
                data.push_back((*it) - '0');
            } else {
                data.push_back((*it) - 'A' + 10);
            }
        }
        pos = 0;
    }

    int get(int len) {
        int ret = 0;
        for(int i=0; i<len; i++) {
            ret = (ret << 1) | ((data[pos>>2] >> (3 - (pos & 3))) & 1);
            pos++;
        }
        return ret;
    }

    std::vector<int> data;
    int pos;
};

class Packet {
    public:
    Packet(BitStream &bs) {
        version = bs.get(3);
        type = bs.get(3);

        if(type == 4) {
            int cont;
            do {
                cont = bs.get(1);
                number = (number << 4) | bs.get(4);
            } while(cont == 1);
        } else {
            int len_id = bs.get(1);
            if(len_id) {
                int packets = bs.get(11);
                for(int i=0; i<packets; i++) {
                    pkts.emplace_back(Packet(bs));
                }
            } else {
                int end = bs.get(15) + bs.pos;
                while(bs.pos < end) {
                    pkts.emplace_back(Packet(bs));
                }
            }
        }
    }

    int sum_version() const {
        int t = version;
        for(auto &p : pkts) {
            t += p.sum_version();
        }
        return t;
    }

    int64_t get_value() const {
        int64_t ret = 0;
        switch(type) {
            case 0: // sum
                ret = std::accumulate(pkts.begin(), pkts.end(), 0LL, [](const int64_t s, const Packet &p) { return s + p.get_value(); });
                break;
            case 1: // product
                ret = std::accumulate(pkts.begin(), pkts.end(), 1LL, [](const int64_t s, const Packet &p) { return s * p.get_value(); });
                break;
            case 2: // min
                ret = std::accumulate(pkts.begin(), pkts.end(), LLONG_MAX, [](const int64_t m, const Packet &p) { return std::min(m, p.get_value()); });
                break;
            case 3: // max
                ret = std::accumulate(pkts.begin(), pkts.end(), LLONG_MIN, [](const int64_t m, const Packet &p) { return std::max(m, p.get_value()); });
                break;
            case 4: // number
                ret = number;
                break;
            case 5: // greater than
                ret = pkts[0].get_value() > pkts[1].get_value() ? 1LL : 0LL;
                break;
            case 6: // less than
                ret = pkts[0].get_value() < pkts[1].get_value() ? 1LL : 0LL;
                break;
            case 7: // equal
                ret = pkts[0].get_value() == pkts[1].get_value() ? 1LL : 0LL;
                break;
            }
        
        return ret;
    }

    private:
    int version = 0;
    int type = 0;
    int64_t number = 0;
    std::vector<Packet> pkts;
};

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input2.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<Packet> pkts;

    while(std::getline(input, line)) {
        BitStream bs(line);
        pkts.emplace_back(bs);
    }

    for(auto &p : pkts) {
        std::cout << "Part1: " << p.sum_version() << "\n";
        std::cout << "Part2: " << p.get_value() << "\n";
    }

    return 0;
}
