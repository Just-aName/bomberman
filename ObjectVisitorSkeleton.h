#pragma once

#include "IObjectVisitor.h"

class ObjectVisitorSkeleton : public IObjectVisitor {
public:
	void operator()(Bomb& /*bomb*/) override {}
	void operator()(Flame& /*flame*/) override {}
	void operator()(Player& /*player*/) override {}
	void operator()(BonusItem& /*bonus*/) override {}
	void operator()(Wall& /*brick*/) override {}
};

class ConstObjectVisitorSkeleton : public IConstObjectVisitor {
public:
	void operator()(Bomb const& /*bomb*/) override {}
	void operator()(Flame const& /*flame*/) override {}
	void operator()(Player const& /*player*/) override {}
	void operator()(BonusItem const& /*bonus*/) override {}
	void operator()(Wall const& /*brick*/) override {}
};
