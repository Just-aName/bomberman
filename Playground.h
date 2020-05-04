#pragma once

#include <functional>
#include <memory> // unique_ptr
#include <vector>

#include "GameObjectFactory.h"
#include "IGameObject.h"

class Dimensions;
class Wall;
class Flame;
class Player;
class Bomb;
class BonusItem;
class Position;
class IObjectVisitor;
class IConstObjectVisitor;

class Playground {
public:
	explicit Playground(Dimensions const &dimensions);

	Wall &createBrick();
	Wall &createStone();
	Flame &createFlame();
	Player &createPlayer();
	Bomb &createBomb();
	BonusItem &createBonusItem(Bonus bonus);

	void killPlayer(Player &player);

	/**
	 * Removes the object from next round.
	 */
	void remove(IGameObject &object);

	/**
	 * Removes the object immediatelly.
	 */
	void removeNow(IGameObject &object);

	bool isValid(Position const &square) const;

	using Predicate = std::function<bool(IGameObject const &)>;

	/**
	 * Iterates over objects that are active in the current tick.
	 * During the iteration, it is possible to add or remove objects.
	 * The added/removed objects will be visible
	 * in the next round.
	 */
	void forEach(std::function<void(IGameObject &)> const &f) const;

	/**
	 * @returns A predicate satisfied by all game objects.
	 */
	static Predicate All();

	/**
	 * @returns A predicate satisfied by all overlaping objects.
	 */
	static Predicate Overlapping(Position const &position);

	/**
	 * Calls given visitor for all active objects satisfying given predicate.
	 */
	void visitAll(IObjectVisitor &visitor, const Predicate &p = All()) const;
	void visitAll(IConstObjectVisitor &visitor, const Predicate &p = All()) const;

	void gameTick();

	bool gameStopped() const;

private:
	void removeExpired();
	void addNewObjects();

	Dimensions const &dimensions_;

	GameObjectFactory factory_;
	std::vector<size_t> oldObjects_;
	std::vector<std::unique_ptr<IGameObject>> currentObjects_;
	std::vector<std::unique_ptr<IGameObject>> newObjects_;
	bool gameStopped_{};
};
