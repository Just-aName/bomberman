#pragma once

#include <memory> // unique_ptr

class Wall;

class Flame;

class Player;

class Bomb;

class BonusItem;

enum class Bonus;

class Playground;

class GameObjectFactory {
public:
	explicit GameObjectFactory(Playground &playground);

	std::unique_ptr<Wall> createBrick() const;

	std::unique_ptr<Wall> createStone() const;

	std::unique_ptr<Flame> createFlame() const;

	std::unique_ptr<Player> createPlayer() const;

	std::unique_ptr<Bomb> createBomb() const;

	std::unique_ptr<BonusItem> createBonusItem(Bonus bonus) const;

private:
	Playground &playground_;
};
