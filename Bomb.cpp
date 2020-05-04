#include "Bomb.h"
#include "Flame.h"
#include "Visitors.h"

struct CheckIfObject : ObjectVisitorSkeleton {
	bool ifWall = false;

	void operator()(Wall &wall) override {
		ifWall = true;
		if (wall.destroyableGetter()) {
			wall.destroyWall();
		}
	}
	void operator()(BonusItem &bonusItem) override {
		bonusItem.remove();
	}

	void operator()(Player &player) override {
		player.killPlayer();
	}
};

Bomb::Bomb(Playground &playground)
    : GameObjectBase(playground) {
}

void Bomb::visit(IObjectVisitor &visitor) {
	visitor(*this);
}

void Bomb::visit(IConstObjectVisitor &visitor) const {
	visitor(*this);
}

void Bomb::position(Position const &value) {
	position_ = value;
}

Position const &Bomb::position() const {
	return position_;
}

TextureRef const &Bomb::texture() const {
	return texture_;
}

void Bomb::texture(TextureRef const &texture) {
	texture_ = texture;
}

void Bomb::doFlame(Position pos, Direction dir) {
	for (int i = 0; i < range_; ++i) {
		pos = next(pos, dir);
        if (!playground_.isValid(pos)) {
            break;
        }
		CheckIfObject visitor;
		playground_.visitAll(visitor, Playground::Overlapping(pos));
		if (!visitor.ifWall) {
			auto &newFlame = playground_.createFlame();
			newFlame.position(pos);
		} else {
			break;
		}
	}
}

void Bomb::removeBomb() {
	if (playerPtr != nullptr) {
		playerPtr->bombCountPlus();
	}
	auto &newflame = playground_.createFlame();
	newflame.position(position_);
	Position pos = position_;
	doFlame(pos, Direction::Up);
	doFlame(pos, Direction::Down);
	doFlame(pos, Direction::Left);
	doFlame(pos, Direction::Right);
	playground_.remove(*this);
}

void Bomb::gameTick() {
	--lifeTime_;
	if (lifeTime_ == 0) {
		removeBomb();
	}
	InFireChecker visitor(position_);
	playground_.visitAll(visitor);
	if (visitor.inFire) {
		removeBomb();
	}
}

void Bomb::setPlayerPtr(Player* player){
	playerPtr = player;
}

void Bomb::setBombRange(int range) {
	range_ = range;
}

Player* Bomb::bombPlayerPtrGetter() {
	return playerPtr;
}

const Player* Bomb::bombPlayerPtrGetter() const{
	return playerPtr;
}

const std::pair<bool,bool>& Bomb::bombPlayerCanMoveGetter() const{
	return playerCanMove;
}

std::pair<bool,bool>& Bomb::bombPlayerCanMoveGetter() {
	return playerCanMove;
}
