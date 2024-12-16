#include "character.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

int main() {
    srand((unsigned)time(0));
    std::cout<<"Welcome to the Enhanced Life Simulation!\nEnter your character's name: ";
    std::string playerName;
    std::getline(std::cin, playerName);

    Character player(playerName);
    player.showMainMenu();
    return 0;
}
