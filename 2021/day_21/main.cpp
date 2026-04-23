#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

struct GameState {
    int p1;
    int p2;
    int p1_score;
    int p2_score;
    int next_go;

    GameState move(int m) {
        if(next_go) {
            int n = (p2 + m) % 10;
            return GameState(p1, n, p1_score, p2_score+n+1, 1-next_go);
        } else {
            int n = (p1 + m) % 10;
            return GameState(n, p2, p1_score+n+1, p2_score, 1-next_go);
        }
    }        

    bool operator<(const GameState &p) const {
        return p1_score == p.p1_score ? (p2_score == p.p2_score ? (p1 == p.p1 ? (p2 == p.p2 ? next_go < p.next_go : p2 < p.p2) : p1 < p.p1) : p2_score < p.p2_score) : p1_score < p.p1_score;
    }
    
    friend std::ostream& operator<<(std::ostream &os, const GameState &g) {
        if(g.next_go) {
            os << "" << (g.p1+1) << "-" << g.p1_score << " _" << (g.p2+1) << "-" << g.p2_score << "_";
        } else {
            os << "_" << (g.p1+1) << "-" << g.p1_score << "_ " << (g.p2+1) << "-" << g.p2_score << "";
        }

        return os;
    }    
};

int calculate_winner(int p1, int p2) {
    GameState gs = {p1-1, p2-1, 0, 0, 0};
    int die = 0;
    int rolls = 0;

    while(true) {
        gs = gs.move((die*3)+6);
        rolls += 3;
        die = (die+3) % 1000;

        if(gs.p2_score >= 1000) {
            return rolls * gs.p1_score;
        } else if(gs.p1_score >= 1000) {
            return rolls * gs.p2_score;
        }
    }

    return 0;
}

int64_t random_winners(int p1, int p2) {
    std::map<GameState,int64_t> exp = {{{p1-1, p2-1, 0, 0, 0}, 1}};
    std::map<int,int> move = {{3,1}, {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1}};
    int64_t win[2] = {0,0};
    const int target = 21;

    while(exp.size() > 0) {
        auto p = exp.extract(exp.begin());

        if(p.key().p2_score >= target || p.key().p1_score >= target) {
            win[p.key().p2_score >= target] += p.mapped();
        } else {
            for(auto &m : move) {
                exp[p.key().move(m.first)] += p.mapped() * m.second;
            } 
        }
    }

    return std::max(win[0], win[1]);
}

int main(int argc, char *argv[]) {
    std::string filename = argc >= 2 ? argv[1] : "test_input.txt";
    std::ifstream input(filename);
    std::string line;

    std::getline(input, line);
    int p1_start = std::stoi(line.substr(28));
    std::getline(input, line);
    int p2_start = std::stoi(line.substr(28));
    
    std::cout << "Part1: " << calculate_winner(p1_start, p2_start) << "\n";
    std::cout << "Part2: " << random_winners(p1_start, p2_start) << "\n";


    return 0;
}
