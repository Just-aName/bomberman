#pragma once

#include "Bonus.h"
#include "GameObjectBase.h"

// (brick, wood, stone, ...)
class Wall : public GameObjectBase {
public:
	Wall(Playground &playground);

	/* < IGameObject > */
	void visit(IObjectVisitor &visitor) override;

	void visit(IConstObjectVisitor &visitor) const override;

	void position(Position const &value) override;

	Position const &position() const override;

	TextureRef const &texture() const override;

	void texture(TextureRef const &texture) override;

	/* </IGameObject > */

	/**
     * @brief Sets the kind of bonus which drops from the wall when destroyed.
     */
	void bonus(Bonus value);

	/**
     * @brief Sets whether the wall is destroyable or not.
     */
	void destroyable(bool value);

	bool destroyableGetter();

	void destroyWall();

private:
	bool destroyable_ = true;
	Bonus bonus_ = Bonus::None;

	void dropBonus();
};
