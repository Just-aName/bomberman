#pragma once

#include <string>

class Playground;
class Player;
class Vector;

Vector getMapSize(std::string const &map);
void fillMapWithItems(
    std::string const &map,
    Playground &playground,
    Player &player1,
    Player &player2);
