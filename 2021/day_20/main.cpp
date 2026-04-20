#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// 4857 too low
// 5022 too high

int ev(std::vector<std::vector<int>> &im, int y, int x, int p, int def) {
    if(y < 0 || y >= im.size() || x < 0 || x >= im[y].size()) {
        return def << p;
    }
    return im[y][x] << p;
}

int count_after(std::vector<int> &map, std::vector<std::vector<int>> image, int count) {
    int width = image[0].size();
    int height = image.size();
    int border = count+2;
    for(auto &v : image) {
        v.insert(v.begin(), border, 0);
        v.insert(v.end(), border, 0);
    }
    image.insert(image.begin(), border, std::vector<int>(width+(border*2), 0));
    image.insert(image.end(), border, std::vector<int>(width+(border*2), 0));
    int inf = 0;

    for(int i=0; i<count; i++) {
        auto n = std::vector<std::vector<int>>(image.size(), std::vector<int>(image[0].size(), 0));  

        for(int y=0; y<image.size(); y++) {
            for(int x=0; x<image[0].size(); x++) {
                int val = ev(image,y-1,x-1,8,inf) | ev(image,y-1,x,7,inf) | ev(image,y-1,x+1,6,inf) | 
                          ev(image,y  ,x-1,5,inf) | ev(image,y  ,x,4,inf) | ev(image,y  ,x+1,3,inf) |
                          ev(image,y+1,x-1,2,inf) | ev(image,y+1,x,1,inf) | ev(image,y+1,x+1,0,inf);
                n[y][x] = map[val];
            }
        }

        inf ^= map[0];
        image.swap(n);
    }

    return std::accumulate(image.begin(), image.end(), 0, [](int t, const auto &v) { return t + std::accumulate(v.begin(), v.end(), 0); });
}


int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::vector<int> map;
    std::getline(input, line);
    for(auto c : line) {
        map.push_back(c == '#' ? 1 : 0);
    }
    
    std::vector<std::vector<int>> image;
    while(std::getline(input, line)) {
        if(line.length() > 0) {
            image.push_back(std::vector<int>());
            for(auto c : line) {
                image.back().push_back(c == '#' ? 1 : 0);
            }
        }
    }
 
    std::cout << "Part1: " << count_after(map, image, 2) << "\n";
    std::cout << "Part2: " << count_after(map, image, 50) << "\n";


    return 0;
}
