#ifndef PROJECT_H
#define PROJECT_H

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <format>

using namespace std;

struct Player {
    int rating;
    int id;
    double score = 0;
    bool paired = false;
    string name;
    explicit Player(int);
    Player(const string&, const string&, const string&, const int&, const int&);
};

ostream& operator<<(ostream&, Player&);

vector <Player> players(int cnt);
vector <pair<Player, Player>> form_tour(const vector <Player>& players, int tour);
unsigned long long cost(const Player& P1, const Player& P2);


#endif