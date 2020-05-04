#pragma once

class Bomb;
class Player;
class Flame;
class BonusItem;
class Wall;

class IObjectVisitor {
public:
	virtual ~IObjectVisitor() = default;

	virtual void operator()(Bomb& bomb) = 0;
	virtual void operator()(Flame& flame) = 0;
	virtual void operator()(Player& player) = 0;
	virtual void operator()(BonusItem& bonusItem) = 0;
	virtual void operator()(Wall& wall) = 0;
};

class IConstObjectVisitor {
public:
	virtual ~IConstObjectVisitor() = default;

	virtual void operator()(Bomb const& bomb) = 0;
	virtual void operator()(Flame const& player) = 0;
	virtual void operator()(Player const& flame) = 0;
	virtual void operator()(BonusItem const& bonusItem) = 0;
	virtual void operator()(Wall const& wall) = 0;
};
