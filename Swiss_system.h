#ifndef PROJECT_H
#define PROJECT_H
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

struct Player {
    int rating;
    double score;
    bool prev_white;
    int id;
    bool is_playing;
    int played_as_white;
    string name;
};

vector <Player> make_table(int players);
vector <vector<bool>> make_played(int players);
vector <pair <Player, Player>> print_tour(vector <Player>& table, vector<vector<bool>>& played, int tour);
void change_params(vector <Player>& table, vector <pair <Player, Player>> cur_tour);
void tournament(vector <Player>& table, vector<vector<bool>>& played, int tours);

#endif