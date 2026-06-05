#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

int score_hand(std::vector<int> &hand) {
    int total = 0;
    for(int i=0; i<hand.size(); i++) {
        total += (hand.size()-i) * hand[i];
    }
    return total;
}

std::pair<int,bool> play_game(std::vector<int> player1, std::vector<int> player2, bool recursive = false) {

    std::set<std::pair<std::vector<int>,std::vector<int>>> history;
    while(true) {
        int p1 = player1[0];
        int p2 = player2[0];

        if(history.contains({player1, player2})) {
            return {0, true};
        }
        history.insert({player1, player2});

        player1.erase(player1.begin());
        player2.erase(player2.begin());

        bool player1_win = p1 > p2;

        if(recursive && p1 <= player1.size() && p2 <= player2.size()) {
            std::vector<int> np1;
            std::vector<int> np2;
            np1.insert(np1.begin(), player1.begin(), player1.begin()+p1);
            np2.insert(np2.begin(), player2.begin(), player2.begin()+p2);

            player1_win = play_game(np1, np2, true).second;
        }

        if(player1_win) {
            player1.push_back(p1);
            player1.push_back(p2);
            if(player2.size() == 0) {
                break;
            }
        } else {
            player2.push_back(p2);
            player2.push_back(p1);
            if(player1.size() == 0) {
                break;
            }
        }
    }

    return {score_hand(player1) + score_hand(player2), player2.size() == 0};
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;
    std::vector<int> player1;
    std::vector<int> player2;

    bool p2 = false;
    while(std::getline(input, line)) {
        if(line.length() == 0) {
            p2 = true;
        } else {
            if(line[0] != 'P') {
                if(p2) {
                    player2.push_back(std::stoi(line));
                } else {
                    player1.push_back(std::stoi(line));
                }
            }
        }
    }

    std::cout << "Part1: " << play_game(player1, player2).first << "\n";
    std::cout << "Part2: " << play_game(player1, player2, true).first << "\n";

    return 0;
}
