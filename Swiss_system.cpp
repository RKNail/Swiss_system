#include "Swiss_system.h"
unsigned int players_cnt;
vector <vector<int>> played;

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
           const int& rating,
           const int& id) : rating(rating), id(id),
          name(name + " " + surname + " " + group) {}

Player::Player(const int id) : rating(0), id(id), name("bye") {
}

vector<Player> players(const int cnt) {
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

    return lst;
}

unsigned long long cost(const Player& p1, const Player& p2) {
    unsigned long long fine = 0;
    fine += played[p1.id][p2.id] * 1000;
    fine += abs(10*p1.score - 10*p2.score);
    fine += abs(p1.rating - p2.rating);
    return fine;
}

