#include "Swiss_system.h"
#include <random>
#include <limits>

using namespace std;

const unsigned int INF = 1e9;
int best_cost = INF;
vector<pair<int, int>> best_matching;
vector<pair<int,int>> current;

unsigned int players_cnt;
vector <vector<int>> played;
vector <Player> players;

bool player_cmp(const Player& p1, const Player& p2) {
    if (p1.score != p2.score) {
        return p1.score > p2.score;
    }
    if (p1.rating != p2.rating) {
        return p1.rating > p2.rating;
    }
    return p1.name > p2.name;
}

ostream& operator<<(ostream& os, Player& P) {
    os << format("Player info: \nName : {0}\nRating : {1}\nScore : {2}\n",
        P.name, P.rating, P.score);
    return os;
}

Player::Player(const string& name,
           const string& surname,
           const string& group,
           const int rating,
           const int id) : rating(rating), id(id),
          name(name + " " + surname + " " + group) {}

Player::Player(const int id) : rating(0), id(id), name("bye") {
}

void registration() {
    int cnt;
    cout << "Введи количество игроков: ";
    cin >> cnt;
    vector<Player> lst;
    lst.reserve(cnt + 1);
    cout << "Вводи параметры каждого игрока следующим образом через пробел:"
            "\n1.Имя\n2.Фамилия\n3.Академическая группа\n4.Рейтинг" << endl;
    for (int i = 0; i < cnt; ++i) {
        string name, surname, group;
        int rating;
        cin >> name >> surname >> group >> rating;
        lst.emplace_back(name, surname, group, rating, i);
    }

    if (cnt & 1) {
        lst.emplace_back(lst.size());
    }

    players_cnt = lst.size();

    played.assign(players_cnt, vector<int>(players_cnt, 0));
    for (int i = 0; i < players_cnt; ++i) {
        played[i][i] = 1e5;
    }
    players = lst;
}

int pair_cost(const Player& p1, const Player& p2) {
    int fine = 0;
    fine += played[p1.id][p2.id] * 1000;
    fine += static_cast<int>(std::abs(p1.score - p2.score) * 200);
    fine += std::abs(p1.rating - p2.rating) * 10;
    return fine;
}

int pick_unpaired() {
    for (int i = 0; i < players_cnt; ++i)
        if (!players[i].paired)
            return i;
    return -1;
}

void dfs(int current_cost) {
    if (current_cost >= best_cost)
        return;

    int u = pick_unpaired();
    if (u == -1) {
        best_cost = current_cost;
        best_matching = current;
        return;
    }

    players[u].paired = true;

    for (int v = u + 1; v < players_cnt; ++v) {
        if (!players[v].paired) {
            players[v].paired = true;

            current.emplace_back(players[u].id, players[v].id);
            dfs(current_cost + pair_cost(players[v], players[u]));
            current.pop_back();

            players[v].paired = false;
        }
    }

    players[u].paired = false;
}

void tournament() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(1, 50);

    int tours;
    cout << "Введи количество туров: ";
    cin >> tours;
    cout << endl << endl;
    registration();
    while (tours--) {
        dfs(0);
        best_cost = INF;
        cout << "Тур №" << tours + 1 << ":" << endl << endl;
        for (auto& it : best_matching) {
            const int random = distrib(gen) & 1;
            played[players[it.first].id][players[it.second].id]++;
            played[players[it.second].id][players[it.first].id]++;
            if (random) {
                cout << players[it.second].name << " " << players[it.first].name << endl;
            } else {
                cout << players[it.first] << " : " << players[it.second] << endl;
            }
        }
    }
    for (auto& it : players) {
        if (it.name != "bye") {
            cout << it << endl;
        }
    }
}