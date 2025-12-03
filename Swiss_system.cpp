#include "Swiss_system.h"

bool law(Player a, Player b) {
    if (a.score - b.score != 0) {
        return a.score > b.score;
    }
    return a.rating > b.rating;
}

vector <Player> make_table(int players) {
    cout << "Напиши имена и рейтинг участников(сначала имя, потом рейтинг)\n";
    vector<Player> table(players);
    for (int i = 0; i < players; ++i) {
        cin >> table[i].name >> table[i].rating;
        table[i].id = i;
        table[i].is_playing = false;
        table[i].prev_white = false;
        table[i].played_as_white = 0;
        table[i].score = 0;
    }
    sort(table.begin(), table.end(), law);
    return table;
}

vector<vector<bool>> make_played(int players) {
    vector<vector<bool>> played(players, vector<bool>(players, false));
    for (int i = 0; i < players; ++i) {
        played[i][i] = true;
    }
    return played;
}

vector<pair<Player, Player>> print_tour(vector <Player>& table, vector<vector<bool>>& played, int tour) {
    cout << "_________________________________ ТУР " << tour << " _________________________________" << endl;
    vector<pair<Player, Player>> TOUR(table.size() / 2);
    int pairs = 0;
    for (int i = 0; i < table.size() && pairs < TOUR.size(); ++i) {
        if (table[i].is_playing) {
            continue;
        }
        table[i].is_playing = true;
        for (int j = i+1; j < table.size(); ++j) {
            if (table[j].is_playing || played[table[j].id][table[i].id]) {
                continue;
            }
            table[j].is_playing = true;
            played[table[i].id][table[j].id] = played[table[j].id][table[i].id] = true;
            if (table[i].prev_white) { // i played as white in previous game
                if (table[j].prev_white) { // j played as white in previous game
                    if (table[j].played_as_white > table[i].played_as_white) {
                        TOUR[pairs].first = table[i];
                        TOUR[pairs].second = table[j];
                        ++pairs;
                        ++table[i].played_as_white;
                        table[i].prev_white = true;
                        table[j].prev_white = false;
                    } else {
                        TOUR[pairs].first = table[j];
                        TOUR[pairs].second = table[i];
                        ++pairs;
                        ++table[j].played_as_white;
                        table[j].prev_white = true;
                        table[i].prev_white = false;
                    }
                } else { // j didn't played as white in previous game
                    TOUR[pairs].first = table[j];
                    TOUR[pairs].second = table[i];
                    ++pairs;
                    ++table[j].played_as_white;
                    table[j].prev_white = true;
                    table[i].prev_white = false;
                }
            } else { // i didn't play as white in previous game
                if (table[j].prev_white) { // j played as white in previous game
                    TOUR[pairs].first = table[i];
                    TOUR[pairs].second = table[j];
                    ++pairs;
                    ++table[i].played_as_white;
                    table[i].prev_white = true;
                    table[j].prev_white = false;
                } else { // j didn't play as white in previous game
                    if (table[j].played_as_white > table[i].played_as_white) {
                        TOUR[pairs] = {table[i], table[j]};
                        ++pairs;
                        ++table[i].played_as_white;
                        table[i].prev_white = true;
                        table[j].prev_white = false;
                    } else {
                        TOUR[pairs] = {table[j], table[i]};
                        ++pairs;
                        ++table[j].played_as_white;
                        table[j].prev_white = true;
                        table[i].prev_white = false;
                    }
                }
            }
            break;
        }
    }
    for (int i = 0; i < TOUR.size(); ++i) {
        cout << TOUR[i].first.name << " VS " << TOUR[i].second.name << endl;
    }
    return TOUR;
}

void change_params(vector <Player> &table, vector <pair <Player, Player>> cur_tour) {
    vector <bool> changed(table.size(), false);
    cout << "Результаты одного игрока из каждой пары:" << endl;
    string p_first_name, p_second_name, res;
    double res_first, res_second;
    for (int i = 0; i < cur_tour.size(); ++i) { // Для каждой пары изменяем параметры
        cin >> p_first_name >> res;
        res_first = res_second = 0;
        for (int j = 0; j < cur_tour.size(); ++j) { // Ищем участников по имени
            if (cur_tour[j].first.name == p_first_name) {
                p_second_name = cur_tour[j].second.name;
                break;
            } else if (cur_tour[j].second.name == p_first_name) {
                p_second_name = cur_tour[j].first.name; 
                break;
            }
        }
        if (res == "победа") {
            res_first = 1;
            res_second = 0;
        } else if (res == "ничья") {
            res_first = 0.5;
            res_second = 0.5;
        } else {
            res_first = 0;
            res_second = 1;
        }
        for (int i = 0; i < table.size(); ++i) {
            if (table[i].name == p_first_name) {
                table[i].score += res_first;
                changed[i] = true;
            } else if (table[i].name == p_second_name) {
                table[i].score += res_second;
                changed[i] = true;
            }
            table[i].is_playing = false;
        }
        for (int i = 0; i < table.size(); ++i) {
            if (!changed[i]) {
                table[i].score += 1;
                break;
            }
        }
    }
}

void tournament(vector <Player>& table, vector<vector<bool>>& played, int tours) {
    for (int i = 1; i <= tours; ++i) {
        vector<pair<Player, Player>> cur_tour = print_tour(table, played, i);
        change_params(table, cur_tour);
        sort(table.begin(), table.end(), law);
    }
    for (int i = 0; i < table.size(); ++i) {
        cout << i + 1 << ". " << table[i].name << " " << table[i].score << endl;
    }
}