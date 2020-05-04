#pragma once

#include <string>

#include "Dimensions.h"
#include "Geometry.h"
#include "Playground.h"

//#define FACTORY_NOT_IMPLEMENTED

// Everything except gui
class Bomberman {
public:
	explicit Bomberman(std::string const &map);

	std::string gameMap;

	Vector const mapSize;
	Dimensions dimensions;
	Playground playground;
#ifndef FACTORY_NOT_IMPLEMENTED
	Player &player1;
	Player &player2;
#endif
};
