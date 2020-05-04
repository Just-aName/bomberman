#include "Player.h"
#include "Visitors.h"

class Flame;

struct CheckBonusObject : ObjectVisitorSkeleton {
	int takeBonusNumber = 0;
	const Position &pos;
	Playground &pGround;

	CheckBonusObject(const Position &pos, Playground &pGround)
			: pos(pos)
			, pGround(pGround) {}

	void operator()(BonusItem &bItem) override {
		if (overlap(bItem.position(), pos)) {
			switch (bItem.bonusGetter()) {
				case Bonus::Poison:
					takeBonusNumber = 1;
					break;
				case Bonus::BombMax:
					takeBonusNumber = 2;
					break;
				case Bonus::BombRange:
					takeBonusNumber = 3;
					break;
				default:
					break;
			}
			pGround.removeNow(bItem);
		}
	}
};


struct CheckBombPosition : ConstObjectVisitorSkeleton {
	const Position &pos;

	CheckBombPosition(const Position &position)
			: pos(position) {}

	bool bombOnPosition = false;

	void operator()(Bomb const &bomb) override {
		if (overlap(bomb.position(), pos)) {
			bombOnPosition = true;
		}
	}
};


struct CanMoveChecker : ConstObjectVisitorSkeleton {
	const Position &pos;
	const Player *ptr;

	CanMoveChecker(const Position &position, const Player *ptr)
			: pos(position)
			, ptr(ptr) {}

	GameObjectPtrs blockingObjects;

	void operator()(Bomb const &bomb) override {
		if (overlap(bomb.position(), pos) && (bomb.bombPlayerPtrGetter() == ptr) && !bomb.bombPlayerCanMoveGetter().first) {
			blockingObjects.emplace_back(&bomb);
		}
		if (overlap(bomb.position(), pos) && (bomb.bombPlayerPtrGetter()!= ptr) && !bomb.bombPlayerCanMoveGetter().second) {
			blockingObjects.emplace_back(&bomb);
		}
	}

	void operator()(Wall const &wall) override {
		if (overlap(wall.position(), pos)) {
			blockingObjects.emplace_back(&wall);
		}
	}
};

struct CheckMoveInsideBomb : ObjectVisitorSkeleton {
	const Position &pos;
	const Player *ptr;

	CheckMoveInsideBomb(const Position &position, const Player *ptr)
			: pos(position)
			, ptr(ptr) {}

	void operator()(Bomb &bomb) override {
		if (!overlap(bomb.position(), pos) && (bomb.bombPlayerPtrGetter() == ptr)) {
			bomb.bombPlayerCanMoveGetter().first = false;
		}
		if (!overlap(bomb.position(), pos) && (bomb.bombPlayerPtrGetter() != ptr)) {
			bomb.bombPlayerCanMoveGetter().second = false;
		}
	}
};

Player::Player(Playground &playground)
    : GameObjectBase(playground) {
}

void Player::visit(IObjectVisitor &visitor) {
	visitor(*this);
}

void Player::visit(IConstObjectVisitor &visitor) const {
	visitor(*this);
}

void Player::position(Position const &value) {
	position_ = value;
}

Position const &Player::position() const {
	return position_;
}

TextureRef const &Player::texture() const {
	return texture_;
}

void Player::texture(TextureRef const &texture) {
	texture_ = texture;
}

bool Player::cantMove(Position position, Direction direction) {
	CanMoveChecker visitor(position, this);
	playground_.visitAll(visitor);
	if (visitor.blockingObjects.empty()) {
		return false;
	}

	if (!(isCentered(position) || visitor.blockingObjects.size() > 1 || overlap(visitor.blockingObjects.front()->position(), round(position) + toVector(direction)))) {
        Vector newDirection = one(round(position).from - position.from);
        position_ = position + newDirection;
	}
	return true;
}

void Player::actualMove(Direction direction) {
	Position newPosition = position_ + toVector(poisonTime_ == 0 ? direction : opposite(direction));
	CheckMoveInsideBomb cmibVisitor(position_, this);
	playground_.visitAll(cmibVisitor);
	if (playground_.isValid(newPosition) && !cantMove(newPosition, direction)) {
		position_ = newPosition;
	}
}

void Player::move(Direction direction, int ticks) {
	// The function `Vector toVector(Direction direction);`
	// from `Geometry.h` could be helpful.
	moveTicks_ = ticks;
	moveDirection_ = direction;
}

bool Player::isMoving() const {
	return (moveTicks_ != 0);
}

void Player::bomb() {
	CheckBombPosition visitor(position_);
	playground_.visitAll(visitor);
	if (bombCount_ > 0 && !visitor.bombOnPosition) {
		auto &newBomb = playground_.createBomb();
		newBomb.position(round(position_));
		newBomb.setBombRange(bombRange_);
		newBomb.setPlayerPtr(this);
		--bombCount_;
	}
}

bool Player::isDead() const {
	return this->isDead_;
}

void Player::killPlayer() {
	isDead_ = true;
	playground_.killPlayer(*this);
}

void Player::applyBonus(int num) {
	switch (num) {
	case 1: //Poison
		this->poisonTime_ = Config::poisonTime;
		break;
	case 2: //BombMax
		++this->bombCount_;
		break;
	case 3: //BombRange
		++this->bombRange_;
		break;
	default:
		break;
	}
}

void Player::gameTick() {
	if (isMoving()) {
		actualMove(moveDirection_);
		--moveTicks_;
	}
	if (poisonTime_ > 0) {
		--poisonTime_;
	}
	CheckBonusObject cboVisitor(position_, playground_);
	playground_.visitAll(cboVisitor);
	if (cboVisitor.takeBonusNumber != 0) {
		applyBonus(cboVisitor.takeBonusNumber);
	}

	InFireChecker visitor(position_);
	playground_.visitAll(visitor);
	if (visitor.inFire) {
		killPlayer();
	}
}

void Player::bombCountPlus() {
	++bombCount_;
}
