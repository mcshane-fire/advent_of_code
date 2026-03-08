#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <numeric>

void print_rows(std::vector<u_int32_t> &rows, int start) {
    std::cout << "\n";
    for(int i=start; i>=0; i--) {
        for(int j=0; j<9; j++) {
            std::cout << (((rows[i] >> j) & 1) ? '#' : '.');
        }
        std::cout << "\n";
    }
    std::cout << "+++++++++\n";
} 

int64_t fall_blocks(std::vector<std::vector<u_int32_t>> &blocks, std::string &line, int64_t n, int64_t every = 0) {
    auto rows = std::vector<u_int32_t>(1024, 257);
    int gas_offset = 0;
    int64_t column_height = 0;
    int64_t c_prev = 0;
    int64_t height_inc = 0;

    std::vector<int> history;

    for(int64_t c=0; c<n; c++) {
        std::vector<u_int32_t> b = blocks[c%blocks.size()];

        if(every > 0 && c % every == 0) {
            int64_t diff = column_height - c_prev;
            //std::cout << c << " " << column_height << " " << diff << ": ";
            c_prev = column_height;

            for(int i=history.size()-1; i>=0; i--) {
                if(history[i] == diff) {                    
                    //std::cout << " " << (history.size() - i);
                    int64_t reps = (n - c) / every;
                    c += reps * every;
                    height_inc += reps * diff;
                    //std::cout << "Accelerated by " << reps << " reps to " << c << " and height " << (column_height+height_inc) << "\n";
                }
            }
            //std::cout << "\n";
            history.push_back(diff);
        }

        rows[column_height & 1023] = 257;
        rows[(column_height+1) & 1023] = 257;
        rows[(column_height+2) & 1023] = 257;

        int64_t y = column_height + 3;
        for(int i=0; i<b.size(); i++) {
            b[i] <<= 3;
            rows[(column_height+3+i) & 1023] = 257;
        }

        while(true) {
            char g = line[gas_offset++];
            if(gas_offset == line.length()) {
                gas_offset = 0;
            }

            bool move = true;
            for(int i=0; i<b.size() && move; i++) {
                u_int32_t bv = g == '>' ? b[i] << 1 : b[i] >> 1;
                if((bv & rows[(y+i) & 1023]) > 0) {
                    move = false;
                }
            }

            if(move) {
                for(int i=0; i<b.size(); i++) {
                    b[i] = g == '>' ? (b[i] << 1) : (b[i] >> 1);
                }
            }

            bool fall = y > 0;
            for(int i=0; i<b.size() && fall; i++) {
                if((b[i] & rows[(y+i-1) & 1023]) > 0) {
                    fall = false;
                }
            }

            if(fall) {
                y--;
            } else {
                break;
            }
        }

        for(int i=0; i<b.size(); i++) {
            rows[(y+i) & 1023] |= b[i];
        }

        column_height = std::max(column_height, (int64_t) (y + b.size()));
    }

    return column_height + height_inc;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::getline(input, line);
    std::vector<std::vector<u_int32_t>> blocks = {{15}, {2,7,2}, {7,4,4}, {1,1,1,1}, {3,3}};

    std::cout << "Part1: " << fall_blocks(blocks, line, 2022) << "\n";

    int interval = std::lcm(blocks.size(), line.length());

    // magic numbers got through inspection of repeated patterns using the lcm as interval
    interval *= (line.length() == 40 ? 7 : 349);

    std::cout << "Part2: " << fall_blocks(blocks, line, 1000000000000, interval) << "\n";

    return 0;
}
