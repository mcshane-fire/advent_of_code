#include <string>
#include <fstream>
#include <iostream>
#include <vector>

struct Node {
    int64_t val = 0;
    struct Node *prev = NULL;
    struct Node *next = NULL;
};

int64_t moves(std::vector<int> ring, int num) {
    int min_label = 1;
    int max_label = ring.size();    
    auto nodes = std::vector<Node>(max_label);
    auto pos = std::vector<int>(max_label);
    for(int i=0; i<ring.size(); i++) {
        pos[ring[i]] = i;
        nodes[i].val = ring[i];
        nodes[i].next = &(nodes[i == max_label-1 ? 0 : i+1]);
        nodes[i].prev = &(nodes[i == 0 ? max_label-1 : i-1]);
    }

    Node *cur = &nodes[0];
    for(int i=0; i<num; i++) {
        Node *start = cur->next;
        Node *end = start->next->next;
        int dest = cur->val == min_label ? max_label : cur->val - 1;

        while(dest == start->val || dest == start->next->val || dest == end->val) {
            dest = dest == min_label ? max_label : dest-1;
        }

        Node *gap = &nodes[pos[dest]];
        
        cur->next = end->next;
        end->next->prev = cur;

        start->prev = gap;
        end->next = gap->next;
        gap->next->prev = end;
        gap->next = start;

        cur = cur->next;
    }

    int64_t ret = 0;
    cur = &nodes[pos[1]];
    if(max_label < 10) {
        for(int i=0; i<max_label-1; i++) {
            cur = cur->next;
            ret = (ret * 10) + cur->val;
        }
    } else {    
        ret = cur->next->val * cur->next->next->val;
    }    

    return ret;
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int> ring;

    std::getline(input, line);
    for(char c : line) {
        ring.push_back(c - '0');
    }

    std::cout << "Part1: " << moves(ring, 100) << "\n";

    int max_label = *std::max_element(ring.begin(), ring.end());
    while(ring.size() < 1000000) {
        ring.push_back(++max_label);
    }

    std::cout << "Part2: " << moves(ring, 10000000) << "\n";

    return 0;
}
