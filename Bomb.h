#pragma once

#include "GameObjectBase.h"

class Bomb : public GameObjectBase {
public:
	Bomb(Playground &playground);

	/* < IGameObject > */
	void visit(IObjectVisitor &visitor) override;

	void visit(IConstObjectVisitor &visitor) const override;

	void position(Position const &value) override;

	Position const &position() const override;

	TextureRef const &texture() const override;

	void texture(TextureRef const &texture) override;

	/* </IGameObject > */

	void gameTick() override;

	void setPlayerPtr(Player* player);

	void setBombRange(int range);

    Player* bombPlayerPtrGetter();

	const Player* bombPlayerPtrGetter() const;

    std::pair<bool,bool>& bombPlayerCanMoveGetter();

	const std::pair<bool,bool>& bombPlayerCanMoveGetter() const;

private:
	Player *playerPtr = nullptr;
	int lifeTime_ = Config::bombTime;
	int range_ = 1;

	std::pair<bool,bool>playerCanMove = {true, true};

	void removeBomb();

	void doFlame(Position pos, Direction dir);
};