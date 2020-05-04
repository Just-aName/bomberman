#pragma once

#include "Bonus.h"
#include "GameObjectBase.h"

class BonusItem : public GameObjectBase {
public:
	BonusItem(Playground &playground);

	/* < IGameObject > */
	void visit(IObjectVisitor &visitor) override;

	void visit(IConstObjectVisitor &visitor) const override;

	void position(Position const &value) override;

	Position const &position() const override;

	TextureRef const &texture() const override;

	void texture(TextureRef const &texture) override;
	/* </IGameObject > */

	/**
 * @brief Sets the kind of bonus.
 */
	void bonus(Bonus value);

	void gameTick() override;

	const Bonus& bonusGetter() const;

	void remove();

private:
	Bonus bonus_ = Bonus::None;
};
