#pragma once

#include "GameObjectBase.h"

using GameObjectPtrs = std::vector<const GameObjectBase *>;

class Player : public GameObjectBase {
public:
	Player(Playground &playground);

	/* < IGameObject > */
	void visit(IObjectVisitor &visitor) override;

	void visit(IConstObjectVisitor &visitor) const override;

	void position(Position const &value) override;

	Position const &position() const override;

	TextureRef const &texture() const override;

	void texture(TextureRef const &texture) override;
	/* </IGameObject > */

	void move(Direction direction, int ticks);

	bool isMoving() const;

	/**
     * @brief Drops a bomb on player's current position.
     */
	void bomb();

	/**
     * @brief Returns true if player is dead.
     */
	bool isDead() const;

	void gameTick() override;

	void bombCountPlus();

	void killPlayer();

private:
	bool isDead_ = false;
	int bombCount_ = 1;
	int bombRange_ = 1;
	int poisonTime_ = 0;
	int moveTicks_ = 0;
	Direction moveDirection_ = Direction::None;

	bool cantMove(Position position, Direction direction);

	void actualMove(Direction direction);

	void applyBonus(int num);
};
