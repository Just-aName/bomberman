#include <Bonus.h>
#include "Config.h"
#include "testhelper.h"
#include "Wall.h"
#include "BonusItem.h"
#include "Flame.h"
#include "Bomb.h"

TEST_CASE("Basic player actions", "[student]") {
	Dimensions dimensions{ { 10, 100 }, Config::mesh };
	Playground playground{ dimensions };
	Player& player = playground.createPlayer();

	SECTION("Move down, no obstacles there") {
		player.position(Position{ Point{ 0, 0 }, 1 });
		REQUIRE(!player.isMoving());

		player.move(Direction::Down, 2);
		REQUIRE(player.position() == Position{ Point{ 0, 0 }, 1 });
		REQUIRE(player.isMoving());

		player.gameTick();
		REQUIRE(player.position() == Position{ Point{ 0, 1 }, 1 });
		REQUIRE(player.isMoving());

		player.gameTick();
		REQUIRE(player.position() == Position{ Point{ 0, 2 }, 1 });
		REQUIRE(!player.isMoving());

		player.gameTick();
		REQUIRE(player.position() == Position{ Point{ 0, 2 }, 1 });
		REQUIRE(!player.isMoving());
	}

	SECTION("Move left, stop on border") {
		player.position(Position{ Point{ 1, 0 }, 1 });
		REQUIRE(!player.isMoving());

		player.move(Direction::Left, 2);
		REQUIRE(player.position() == Position{ Point{ 1, 0 }, 1 });
		REQUIRE(player.isMoving());

		player.gameTick();
		REQUIRE(player.position() == Position{ Point{ 0, 0 }, 1 });
		REQUIRE(player.isMoving());

		player.gameTick();
		REQUIRE(player.position() == Position{ Point{ 0, 0 }, 1 });
		REQUIRE(!player.isMoving());

		player.gameTick();
		REQUIRE(player.position() == Position{ Point{ 0, 0 }, 1 });
		REQUIRE(!player.isMoving());
	}

	
	SECTION("Drop a bomb") {
		player.position(dimensions.coarsePosition(3, 2));
		player.bomb();
		REQUIRE(!containsBomb(playground, player.position()));
		playground.gameTick();
		REQUIRE(containsBomb(playground, player.position()));
	}

	SECTION("Bomb destroys brick wall") {
		Position bombPos = Position{ Point{ 1, 0 }, 1};
		Position wallPos = Position{ Point{ 1, 1 }, 1};
		player.position(bombPos);
		Wall& wall = playground.createBrick();
		wall.position(wallPos);
		player.bomb();
		player.move(Direction::Down, 3);
		playground.gameTick();
		CHECK(containsWall(playground, wallPos));
		CHECK(containsBomb(playground, bombPos));
		for (int i = 0; i < Config::bombTime - 2; ++i) {
			playground.gameTick();
		}
		CHECK(containsBomb(playground, bombPos));
		playground.gameTick();
		CHECK(containsBomb(playground, bombPos));
		CHECK(containsWall(playground, wallPos));
		playground.gameTick();
		CHECK(!containsBomb(playground, bombPos));
		CHECK(!containsWall(playground, wallPos));
	}

	SECTION("Bonus item does not stop fire") {
		Position bonusPos = Position{ Point{ 1, 1 }, 1};
		Position bombPos = Position{ Point{ 1, 0 }, 1};
		player.position(bombPos);
		BonusItem& bonus = playground.createBonusItem(Bonus::BombRange);
		bonus.position(bonusPos);
		player.bomb();
		player.move(Direction::Right, 3);
		playground.gameTick();
		CHECK(containsBomb(playground, bombPos));
		CHECK(containsBonusItem(playground, bonusPos));
		for (int i = 0; i < Config::bombTime - 1; ++i) {
			playground.gameTick();
		}
		CHECK(containsBomb(playground, bombPos));
		CHECK(containsBonusItem(playground, bonusPos));
		playground.gameTick();
		CHECK(!containsBomb(playground, bombPos));
		CHECK(!containsBonusItem(playground, bonusPos));
		CHECK(containsFlame(playground, bonusPos));
		for (int j = 0; j < Config::fireTime; ++j) {
			playground.gameTick();
		}
		CHECK(!containsFlame(playground, bonusPos));
		CHECK(!containsFlame(playground, Position{ Point{ 1, 2 }, 1}));
		playground.gameTick();
		CHECK(!containsFlame(playground, bonusPos));
	}

	SECTION("The flame will eventually disappear") {
		Position flamePos = Position{ Point{ 1, 1 }, 1};
		Flame& flame = playground.createFlame();
		flame.position(flamePos);
		CHECK(!containsFlame(playground, flamePos));
		playground.gameTick();
		CHECK(containsFlame(playground, flamePos));
		for (int i = 0; i < Config::fireTime; ++i) {
			playground.gameTick();
		}
		CHECK(!containsFlame(playground, flamePos));
	}

	SECTION("Bonus::BombRange") {
		Position bombPos = Position{ Point{ 2, 2 }, 1};
		player.position(Position{ Point{ 1, 2 }, 1});
		BonusItem& bonus = playground.createBonusItem(Bonus::BombRange);
		bonus.position(bombPos);
		player.move(Direction::Right, 1);
		playground.gameTick();
		CHECK(player.isMoving());
		playground.gameTick();
		player.bomb();
		CHECK(player.position() == bombPos);
		playground.gameTick();
		CHECK(containsBomb(playground, bombPos));
		for (int i = 0; i < Config::bombTime - 1; ++i) {
			playground.gameTick();
		}
		CHECK(containsBomb(playground, bombPos));
		playground.gameTick();
		CHECK(!containsBomb(playground, bombPos));
		CHECK(containsFlame(playground, bombPos));
		CHECK(containsFlame(playground, Position{ Point{ 0, 2 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 1, 2 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 3, 2 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 4, 2 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 2, 0 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 2, 1 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 2, 3 }, 1}));
		CHECK(containsFlame(playground, Position{ Point{ 2, 4 }, 1}));
		for (int j = 0; j < Config::fireTime; ++j) {
			playground.gameTick();
		}
		CHECK(!containsFlame(playground, bombPos));
		CHECK(!containsFlame(playground, Position{ Point{ 0, 2 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 1, 2 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 3, 2 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 4, 2 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 2, 0 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 2, 1 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 2, 3 }, 1}));
		CHECK(!containsFlame(playground, Position{ Point{ 2, 4 }, 1}));
	}

	SECTION("Bonus::Poison 1") {
		Position bonusPos = Position{ Point{ 2, 2 }, 1};
		player.position(Position{ Point{ 1, 2 }, 1});
		BonusItem& bonus = playground.createBonusItem(Bonus::Poison);
		bonus.position(bonusPos);
		playground.gameTick();
		player.move(Direction::Right, 1);
		playground.gameTick();
		player.move(Direction::Up, Config::poisonTime + 1);
		playground.gameTick();
		CHECK(player.position() == Position{ Point{ 2, 3 }, 1});
		for (int i = 0; i < Config::poisonTime; ++i) {
			playground.gameTick();
		}
		CHECK(player.position() == Position{ Point{ 2, 3 + Config::poisonTime - 2 }, 1});
	}

	SECTION("Bonus::Poison 2") {
		Position bonusPos = Position{ Point{ 2, 2 }, 1};
		player.position(Position{ Point{ 1, 2 }, 1});
		BonusItem& bonus = playground.createBonusItem(Bonus::Poison);
		bonus.position(bonusPos);
		playground.gameTick();
		player.move(Direction::Right, 2);
		playground.gameTick();
		playground.gameTick();
		CHECK(player.position() == Position{ Point{ 1, 2 }, 1});
		player.move(Direction::Up, Config::poisonTime - 1);
		for (int i = 0; i < Config::poisonTime - 1; ++i) {
			playground.gameTick();
		}
		CHECK(player.position() == Position{ Point{ 1, 2 + Config::poisonTime - 1 }, 1});
	}

	SECTION("Bonus::BombMax 1") {
		player.position(Position{ Point{ 1, 2 }, 1});
		BonusItem& bonus1 = playground.createBonusItem(Bonus::BombMax);
		bonus1.position(Position{ Point{ 2, 2 }, 1});
		BonusItem& bonus2 = playground.createBonusItem(Bonus::BombMax);
		bonus2.position(Position{ Point{ 3, 2 }, 1});
		playground.gameTick();
		player.move(Direction::Right, 2);
		playground.gameTick();
		playground.gameTick();
		player.bomb();
		player.move(Direction::Right, 3);
		playground.gameTick();
		player.bomb();
		playground.gameTick();
		player.bomb();
		playground.gameTick();
		CHECK(containsBomb(playground, Position{ Point{ 3, 2 }, 1}));
		CHECK(containsBomb(playground, Position{ Point{ 4, 2 }, 1}));
		CHECK(containsBomb(playground, Position{ Point{ 5, 2 }, 1}));
		player.bomb();
		playground.gameTick();
		CHECK(!containsBomb(playground, Position{ Point{ 6, 2 }, 1}));
	}

	SECTION("Bonus::BombMax 2") {
		player.position(Position{ Point{ 1, 2 }, 1});
		BonusItem& bonus1 = playground.createBonusItem(Bonus::BombMax);
		bonus1.position(Position{ Point{ 2, 2 }, 1});
		playground.gameTick();
		player.move(Direction::Right, 1);
		playground.gameTick();
		player.bomb();
		playground.gameTick();
		player.bomb();
		player.move(Direction::Right, 1);
		playground.gameTick();
		player.bomb();
		playground.gameTick();
		CHECK(containsBomb(playground, Position{ Point{ 2, 2 }, 1}));
		CHECK(containsBomb(playground, Position{ Point{ 3, 2 }, 1}));
	}

	SECTION("Bomb detonates other bomb") {
		player.position(Position{ Point{ 1, 2 }, 1});
		playground.gameTick();
		player.bomb();
		playground.gameTick();
		playground.gameTick();
		Bomb& bomb = playground.createBomb();
		bomb.position(Position{ Point{ 2, 2 }, 1});
		for (int i = 0; i < Config::bombTime - 2; ++i) {
			playground.gameTick();
		}
		CHECK(containsBomb(playground, Position{ Point{ 1, 2 }, 1}));
		CHECK(containsBomb(playground, Position{ Point{ 2, 2 }, 1}));
		playground.gameTick();
		CHECK(!containsBomb(playground, Position{ Point{ 1, 2 }, 1}));
		CHECK(containsBomb(playground, Position{ Point{ 2, 2 }, 1}));
		playground.gameTick();
		CHECK(!containsBomb(playground, Position{ Point{ 2, 2 }, 1}));
	}

	SECTION("Wall does not break move") {
		player.position(Position{ Point{ 1, 2 }, 1});
		Wall& wall = playground.createBrick();
		wall.position(Position{ Point{ 1, 3 }, 1});
		playground.gameTick();
		player.move(Direction::Down, Config::bombTime + 2);
		Bomb& bomb = playground.createBomb();
		bomb.position(Position{ Point{ 1, 4 }, 1});
		for (int i = 0; i < Config::bombTime; ++i) {
			playground.gameTick();
		}
		CHECK(player.isMoving());
		playground.gameTick();
		CHECK(!containsWall(playground, Position{ Point{ 1, 3 }, 1}));
		playground.gameTick();
		CHECK(player.position() == Position{ Point{ 1, 3 }, 1});
		CHECK(!player.isMoving());
		CHECK(!player.isDead());
	}
}
