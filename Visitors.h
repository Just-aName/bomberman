#pragma once

// A collection of useful visitors.
#include "Bomb.h"
#include "BonusItem.h"
#include "Flame.h"
#include "ObjectVisitorSkeleton.h"
#include "Player.h"
#include "Wall.h"

struct DeadPlayerDetector : ConstObjectVisitorSkeleton {
	bool detected = false;

	void operator()(Player const &player) override { detected = detected || player.isDead(); }
};

struct InFireChecker : ConstObjectVisitorSkeleton {
	const Position &pos;

	InFireChecker(const Position &position)
	    : pos(position) {}

	bool inFire = false;

	void operator()(Flame const &flame) override {
		if (overlap(flame.position(), pos)) {
			inFire = true;
		}
	}
};