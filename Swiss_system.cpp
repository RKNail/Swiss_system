#include "Swiss_system.h"
#include <random>
#include <limits>
#include <format>

using namespace std;

const unsigned int INF = 1e9;
int best_cost = INF;
vector<pair<int, int>> best_matching;
vector<pair<int, int>> current;

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
    os << "Player info: \nName: " << P.name << endl << "Score: " << P.score << endl << "Rating: " << P.rating << endl;
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
    lst.reserve(cnt + (cnt&1));
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
    fine += played[p1.id][p2.id] * 10000;
    fine += static_cast<int>(std::abs(p1.score - p2.score) * 1000);
    fine += std::abs(p1.rating - p2.rating);
    return fine;
}

int pick_unpaired(const vector<int>& order) {
    for (int idx : order)
        if (!players[idx].paired)
            return idx;
    return -1;
}


void dfs(int current_cost, const vector<int>& order) {
    if (current_cost >= best_cost) return;

    int u = pick_unpaired(order);
    if (u == -1) {
        best_cost = current_cost;
        best_matching = current;
        return;
    }

    players[u].paired = true;

    for (int v : order) {
        if (!players[v].paired) {
            players[v].paired = true;
            current.emplace_back(u, v);
            dfs(current_cost + pair_cost(players[u], players[v]), order);
            current.pop_back();
            players[v].paired = false;
        }
    }

    players[u].paired = false;
}


void tournament() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(0, 1);
    int tours;
    cout << "Введи количество туров: ";
    cin >> tours;
    cout << endl << endl;
    registration();
    vector<int> order(players_cnt);
    iota(order.begin(), order.end(), 0);
    while (tours--) {
        best_cost = INF;
        best_matching.clear();

        for (auto& p : players)
            p.paired = false;

        sort(order.begin(), order.end(), [&](int a, int b) {
            if (players[a].score != players[b].score)
                return players[a].score > players[b].score;
            return players[a].rating > players[b].rating;
        });

        dfs(0, order);
        cout << "Тур №" << tours + 1 << ":" << endl << endl;
        for (auto& it : best_matching) {
            const int random = distrib(gen);
            played[players[it.first].id][players[it.second].id]++;
            played[players[it.second].id][players[it.first].id]++;
            if (!random) {
                cout << players[it.second].id << "." << players[it.second].name << " : "
                << players[it.first].id << "." << players[it.first].name << endl;
            } else {
                cout << players[it.first].id << "." << players[it.first].name << " : "
                << players[it.second].id << "." << players[it.second].name << endl;
            }
        }
        cout << endl;
        cout << "Введи результаты одного игрока из каждой пары(id и Победа/Ничья/Поражение)" << endl;
        string result;
        int id;
        for (int i = 0; i < players_cnt / 2; ++i) {
            while (true) {
                cin >> id >> result;
                if (result == "Победа" or result == "Ничья" or result == "Поражение") {
                    break;
                }
                cout << "Введи правильный результат\n";
            }
            for (auto& it : best_matching) {
                if (id == it.first) {
                    if (result == "Победа") {
                        players[it.first].score += 1;
                    } else if (result == "Ничья") {
                        players[it.first].score += 0.5;
                        players[it.second].score += 0.5;
                    } else if (result == "Поражение") {
                        players[it.second].score += 1;
                    }
                    break;
                } else if (id == it.second) {
                    if (result == "Победа") {
                        players[it.second].score += 1;
                    } else if (result == "Ничья") {
                        players[it.first].score += 0.5;
                        players[it.second].score += 0.5;
                    } else if (result == "Поражение") {
                        players[it.first].score += 1;
                    }
                }
            }
        }
    }
    sort(players.begin(), players.end(),
        [](const Player& a, const Player& b) {
           return a.score > b.score;
        });
    for (auto& it : players) {
        if (it.name != "bye") {
            cout << it.name << ": " << it.score << endl;
        }
    }
}