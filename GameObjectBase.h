#pragma once
#include "Config.h"
#include "Geometry.h"
#include "IGameObject.h"
#include "Playground.h"
#include "TextureRef.h"
#include <utility> // move


class GameObjectBase : public IGameObject {
protected:
	Playground &playground_;
	Position position_{};
	TextureRef texture_;

public:
	GameObjectBase(Playground &playground)
	    : playground_(playground) {}

	
	void gameTick() override {}

	// void visit(IObjectVisitor & visitor) override {}
	// void visit(IConstObjectVisitor & visitor) const override {}
};
