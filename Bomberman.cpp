#include "Config.h"
#include "MapParser.h"

#include "Bomberman.h"

Bomberman::Bomberman(std::string const &map)
    : gameMap(map)
    , mapSize(getMapSize(gameMap))
    , dimensions(mapSize, Config::mesh)
    , playground(dimensions)
#ifndef FACTORY_NOT_IMPLEMENTED
    , player1(playground.createPlayer())
    , player2(playground.createPlayer())
#endif
{
#ifndef FACTORY_NOT_IMPLEMENTED
	fillMapWithItems(gameMap, playground, player1, player2);
#endif
}
