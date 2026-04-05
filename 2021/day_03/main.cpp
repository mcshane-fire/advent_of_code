#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

int get_number(std::map<std::string,int> &freq, bool big) {
    int i=0;
    int ret=0;
    std::string s = "";
    while(true) {
        std::cout << s << " " << freq[s+"1"] << " " << freq[s+"0"] << "\n";

        if(freq[s+"1"] == 0 && freq[s+"0"] == 0) {
            break;
        }
    
        ret <<= 1;

        if((big && freq[s+"1"] >= freq[s+"0"]) || (!big && ((freq[s+"1"] > 0 && freq[s+"1"] < freq[s+"0"]) || freq[s+"0"] == 0))) {
            s+="1";
            ret+=1;
            i++;
        } else {
            s+="0";
            i++;
        }
    }

    return ret;
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::map<std::string,int> freq;
    int cur = 0;

    while(std::getline(input, line)) {
        for(int i=1; i<=line.length(); i++) {
            auto s = line.substr(0,i);
            freq[s]+=1;
        }
    }

    std::cout << "Part2: " << get_number(freq, true) * get_number(freq, false) << "\n";

    return 0;
}
