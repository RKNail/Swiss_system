#include "Swiss_system.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int players, tours;
    std::cout << "Напиши сколько игроков и туров(сначала игроков, потом туров):";
    std::cin >> players >> tours;
    std::cout << std::endl;
    std::vector<Player> table = make_table(players);
    std::vector<std::vector<bool>> played = make_played(players);
    tournament(table, played, tours);
    return 0;
}