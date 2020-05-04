#include <algorithm> // std::max

#include "Bonus.h"
#include "Config.h"
#include "Geometry.h"
#include "Player.h"
#include "Playground.h"
#include "Wall.h"

#include "MapParser.h"

Vector getMapSize(std::string const &map) {
	Vector v{ 0, 0 };
	int currx = 0;
	for (char c : map) {
		if (c == '\n') {
			v.x = std::max(v.x, currx);
			currx = 0;
			continue;
		}

		if (currx == 0) {
			v.y++;
		}
		currx++;
	}
	return v;
}

void fillMapWithItems(std::string const &map,
    Playground &playground,
    Player &player1,
    Player &player2) {
	Position square{ { 0, 0 }, Config::mesh };

	for (char c : map) {
		switch (c) {
		case '\n':
			square.from.y += Config::mesh;
			square.from.x = 0;
			continue;

		case 's': // stone
			playground.createStone().position(square);
			break;

		case 'b': // wall
			playground.createBrick().position(square);
			break;

		case '1': // player1
			player1.position(square);
			break;

		case '2': // player2
			player2.position(square);
			break;

		case 'r': // bomb range bonus (hidden behind wall)
		{
			Wall &wall = playground.createBrick();
			wall.position(square);
			wall.bonus(Bonus::BombRange);
			break;
		}
		case 'p': // poisoned bonus
		{
			Wall &wall = playground.createBrick();
			wall.position(square);
			wall.bonus(Bonus::Poison);
			break;
		}
		case 'M': // bomb Maximum bonus (behind wall)
		{
			Wall &wall = playground.createBrick();
			wall.position(square);
			wall.bonus(Bonus::BombMax);
			break;
		}
		default:
			break;
		}
		square.from.x += Config::mesh;
	}
}
