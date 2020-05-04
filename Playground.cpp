#include <algorithm> // remove_if
#include <cassert> // assert
#include <utility> // move

#include "Bomb.h"
#include "BonusItem.h"
#include "Dimensions.h"
#include "Flame.h"
#include "Player.h"
#include "Wall.h"

#include "Playground.h"

namespace {
bool isEmpty(std::unique_ptr<IGameObject> const &ptr) {
	return ptr == nullptr;
}
} // namespace

Playground::Playground(Dimensions const &dimensions)
    : dimensions_(dimensions)
    , factory_(*this) {
}

bool Playground::isValid(Position const &square) const {
	return dimensions_.isValid(square);
}

/**
 * Templates would reduce source code size here.
 */

Wall &Playground::createBrick() {
	std::unique_ptr<Wall> own = factory_.createBrick();
	Wall &r = *own;
	newObjects_.push_back(std::move(own));
	return r;
}

Wall &Playground::createStone() {
	std::unique_ptr<Wall> own = factory_.createStone();
	Wall &r = *own;
	newObjects_.push_back(std::move(own));
	return r;
}

Flame &Playground::createFlame() {
	std::unique_ptr<Flame> own = factory_.createFlame();
	Flame &r = *own;
	newObjects_.push_back(std::move(own));
	return r;
}

Player &Playground::createPlayer() {
	std::unique_ptr<Player> own = factory_.createPlayer();
	Player &r = *own;
	newObjects_.push_back(std::move(own));
	return r;
}

Bomb &Playground::createBomb() {
	std::unique_ptr<Bomb> own = factory_.createBomb();
	Bomb &r = *own;
	newObjects_.push_back(std::move(own));
	return r;
}

BonusItem &Playground::createBonusItem(Bonus bonus) {
	std::unique_ptr<BonusItem> own = factory_.createBonusItem(bonus);
	BonusItem &r = *own;
	newObjects_.push_back(std::move(own));
	return r;
}

void Playground::forEach(std::function<void(IGameObject &)> const &f) const {
	for (auto const &up : currentObjects_) {
		f(*up);
	}
}

Playground::Predicate Playground::All() {
	return [](IGameObject const &) { return true; };
}

Playground::Predicate Playground::Overlapping(Position const &position) {
	return [position](IGameObject const &object) {
		return overlap(position, object.position());
	};
}

void Playground::visitAll(IObjectVisitor &visitor, Predicate const &p) const {
	for (auto const &object : currentObjects_) {
		if (object && p(*object))
			object->visit(visitor);
	}
}

void Playground::visitAll(IConstObjectVisitor &visitor, Predicate const &p) const {
	for (auto const &object : currentObjects_) {
		if (object && p(*object))
			object->visit(visitor);
	}
}

void Playground::gameTick() {
	for (auto const &object : currentObjects_) {
		if (object)
			object->gameTick();
	}
	removeExpired();
	addNewObjects();
}

void Playground::killPlayer(Player & /*player*/) {
	gameStopped_ = true;
}

void Playground::remove(IGameObject &object) {
	size_t idx = 0;
	for (auto &current : currentObjects_) {
		if (current.get() == &object) {
			oldObjects_.push_back(idx);
			return;
		}
		idx++;
	}
}

void Playground::removeNow(IGameObject &object) {
	for (auto &current : currentObjects_) {
		if (current.get() == &object) {
			current.reset();
			return;
		}
	}
}

void Playground::removeExpired() {
	for (size_t idx : oldObjects_) {
		assert(idx < currentObjects_.size());
		currentObjects_[idx].reset();
	}
	oldObjects_.clear();

	auto const from = std::remove_if(currentObjects_.begin(), currentObjects_.end(), isEmpty);
	currentObjects_.erase(from, currentObjects_.end());
}

void Playground::addNewObjects() {
	for (std::unique_ptr<IGameObject> &p : newObjects_) {
		currentObjects_.push_back(std::move(p));
	}
	newObjects_.clear();
}

bool Playground::gameStopped() const {
	return gameStopped_;
}
