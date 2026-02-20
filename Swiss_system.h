#ifndef SWISS_SYSTEM_H
#define SWISS_SYSTEM_H
#include <iostream>
#include <vector>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <algorithm>

class Player {
    std::string name, surname, group;
    int id, rating;
    double score;
    public:
    Player();
    Player(std::string, std::string, std::string, int, int);
    ~Player();
};

class Tournament {
    int player_cnt;

    std::vector<Player>& players;
    public:
    Tournament();
    Tournament(int, int, std::vector<Player>&);
    std::vector<std::pair<int, int>> form_tour(std::vector<Player>&, int);
    void show_tour(std::vector<std::pair<int, int>>);

    ~Tournament();
}
#endif