#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <map>
#include <numeric>

struct counting {
    uint max_run;
    uint next_run;
    uint current_run;
    uint wildcards;
    char last_char;
};

struct counting count_reps_op(struct counting& lhs, char rhs) {
    if(lhs.last_char == rhs) {
        lhs.current_run++;
    } else {
        lhs.current_run = 1;
        lhs.last_char = rhs;
    }

    if(lhs.current_run > lhs.max_run) {
        lhs.max_run = lhs.current_run;
    } else if (lhs.current_run > lhs.next_run) {
        lhs.next_run = lhs.current_run;
    }

    return lhs;
}

uint score_card(std::string card) {
    std::map<char,uint> score = {{'2',2}, {'3',3}, {'4',4}, {'5',5}, {'6',6}, {'7',7}, {'8',8},
                                {'9',9}, {'T',10}, {'J',11}, {'Q',12}, {'K',13}, {'A',14}};
                            
    uint sum = std::accumulate(card.begin(), card.end(), 0, [&score](uint lhs, char rhs) { return (lhs<<4) + score[rhs]; });

    std::sort(card.begin(), card.end());
    struct counting rec = {0,0,0,0,0};
    struct counting rec2 = std::accumulate(card.begin(), card.end(), rec, count_reps_op);
    
    return (rec2.max_run << 24) | (rec2.next_run << 20) | sum;
}

struct counting count_reps_wildcards_op(struct counting& lhs, char rhs) {
    if(rhs == 'J') {
        lhs.wildcards++;
    } else {
        if(lhs.last_char == rhs) {
            lhs.current_run++;
        } else {
            lhs.current_run = 1;
            lhs.last_char = rhs;
        }

        if(lhs.current_run > lhs.max_run) {
            lhs.max_run = lhs.current_run;
        } else if (lhs.current_run > lhs.next_run) {
            lhs.next_run = lhs.current_run;
        }
    }

    return lhs;
}

uint score_wildcards(std::string card) {
    std::map<char,uint> score = {{'J',1}, {'2',2}, {'3',3}, {'4',4}, {'5',5}, {'6',6}, {'7',7}, {'8',8},
                                {'9',9}, {'T',10},{'Q',12}, {'K',13}, {'A',14}};

    uint sum = std::accumulate(card.begin(), card.end(), 0, [&score](uint lhs, char rhs) { return (lhs<<4) + score[rhs]; });

    std::sort(card.begin(), card.end());
    struct counting rec = {0,0,0,0,0};
    struct counting rec2 = std::accumulate(card.begin(), card.end(), rec, count_reps_wildcards_op);

    return ((rec2.max_run + rec2.wildcards) << 24) | (rec2.next_run << 20) | sum;
}

uint64_t total_winnings(std::vector<std::pair<uint,uint>>& cards) {
    uint64_t total = 0;

    std::sort(cards.begin(), cards.end(), [](const std::pair<uint,uint>& lhs, const std::pair<uint,uint>& rhs) { return lhs.first < rhs.first; });

    for(uint i=0; i<cards.size(); i++) {        
        //std::cout << cards[i].first << " " << cards[i].second << "\n";
        total += (i+1) * cards[i].second;
    }

    return total;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;

    std::vector<std::pair<uint,uint>> cards;
    std::vector<std::pair<uint,uint>> cards_wildcards;

    while(std::getline(input, line)) {
        std::stringstream ss(line);
        std::string card, wager;
        ss >> card;
        ss >> wager;

        cards.emplace_back(score_card(card), std::stoul(wager));
        cards_wildcards.emplace_back(score_wildcards(card), std::stoul(wager));
    }

    std::cout << "Part1: " << total_winnings(cards) << "\n";
    std::cout << "Part2: " << total_winnings(cards_wildcards) << "\n";
    
    return 0;
}