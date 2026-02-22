#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <format>

struct Player {
    int rating;
    int id;
    double score = 0;
    bool paired = false;
    std::string name;
    explicit Player(int);
    Player(const std::string&, const std::string&, const std::string&, int, int);
};

std::ostream& operator<<(std::ostream&, Player&);

void registration();
void tournament();
int pair_cost(const Player& P1, const Player& P2);


#endif