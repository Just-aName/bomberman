#pragma once

#include "catch.hpp"

#include "Dimensions.h"
#include "Player.h"
#include "Playground.h"

#include "ObjectVisitorSkeleton.h"

inline bool containsBomb(Playground const &playground, Position const &position) {
	struct Visitor final : ConstObjectVisitorSkeleton {
		bool ret = false;
		void operator()(const Bomb & /*bomb*/) override { ret = true; }
	};

	Visitor visitor;
	auto match = [&](IGameObject const &object) {
		return overlap(position, object.position());
	};
	playground.visitAll(visitor, match);
	return visitor.ret;
}

inline bool containsFlame(Playground const &playground, Position const &position) {
	struct Visitor final : ConstObjectVisitorSkeleton {
		bool ret = false;
		void operator()(const Flame & /*flame*/) override { ret = true; }
	};

	Visitor visitor;
	auto match = [&](IGameObject const &object) {
		return overlap(position, object.position());
	};
	playground.visitAll(visitor, match);
	return visitor.ret;
}

inline bool containsBonusItem(Playground const &playground, Position const &position) {
	struct Visitor final : ConstObjectVisitorSkeleton {
		bool ret = false;
		void operator()(const BonusItem & /*bonusItem*/) override { ret = true; }
	};

	Visitor visitor;
	auto match = [&](IGameObject const &object) {
		return overlap(position, object.position());
	};
	playground.visitAll(visitor, match);
	return visitor.ret;
}

inline bool containsWall(Playground const &playground, Position const &position) {
	struct Visitor final : ConstObjectVisitorSkeleton {
		bool ret = false;
		void operator()(const Wall & /*wall*/) override { ret = true; }
	};

	Visitor visitor;
	auto match = [&](IGameObject const &object) {
		return overlap(position, object.position());
	};
	playground.visitAll(visitor, match);
	return visitor.ret;
}
