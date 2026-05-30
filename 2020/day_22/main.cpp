#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int score_hand(std::vector<int> &hand) {
    int total = 0;
    for(int i=0; i<hand.size(); i++) {
        total += (hand.size()-i) * hand[i];
    }
    return total;
}

int play_game(std::vector<int> &player1, std::vector<int> &player2) {
    while(true) {
        int p1 = player1[0];
        int p2 = player2[0];

        player1.erase(player1.begin());
        player2.erase(player2.begin());

        if(p1 > p2) {
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

    return score_hand(player1) + score_hand(player2);
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

    std::cout << "Part1: " << play_game(player1, player2) << "\n";

    return 0;
}
