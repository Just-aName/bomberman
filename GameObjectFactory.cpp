#include "Bomb.h"
#include "Bonus.h"
#include "BonusItem.h"
#include "Flame.h"
#include "Player.h"
#include "Wall.h"

#include "GameObjectFactory.h"

GameObjectFactory::GameObjectFactory(Playground &playground)
    : playground_(playground) {
}

std::unique_ptr<Wall> GameObjectFactory::createBrick() const {
	// Brick is a destroyable wall.
	// It can be destroed by an explosion of a bomb.

	// Set those textures on the objects created there.
	TextureRef texture{ "brick_gray0.png" };
	auto wall = Wall(playground_);
	wall.texture(texture);
	return std::make_unique<Wall>(wall);
}

std::unique_ptr<Wall> GameObjectFactory::createStone() const {
	// Stone is a non-destroyable wall.
	TextureRef texture{ "stone2_gray0.png" };
	auto wall = Wall(playground_);
	wall.texture(texture);
	wall.destroyable(false);
	return std::make_unique<Wall>(wall);
}

std::unique_ptr<Flame> GameObjectFactory::createFlame() const {
	TextureRef texture{ "conjure_flame.png" };
	auto flame = Flame(playground_);
	flame.texture(texture);
	return std::make_unique<Flame>(flame);
}

std::unique_ptr<Player> GameObjectFactory::createPlayer() const {
	TextureRef texture{ "human.png" };
	auto player = Player(playground_);
	player.texture(texture);
	return std::make_unique<Player>(player);
}

std::unique_ptr<Bomb> GameObjectFactory::createBomb() const {
	TextureRef texture{ "delayed_fireball.png" };
	auto bomb = Bomb(playground_);
	bomb.texture(texture);
	return std::make_unique<Bomb>(bomb);
}

std::unique_ptr<BonusItem> GameObjectFactory::createBonusItem(Bonus bonus) const {
	TextureRef texture = [&] {
		switch (bonus) {
		case Bonus::Poison:
			return TextureRef{ "potion_bubbly.png" };
		default:
			return TextureRef{ "sticky_flame.png" };
		}
	}();
	auto bonusItem = BonusItem(playground_);
	bonusItem.texture(texture);
	bonusItem.bonus(bonus);
	return std::make_unique<BonusItem>(bonusItem);
}