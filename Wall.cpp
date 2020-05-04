#include "Wall.h"
#include "BonusItem.h"
#include "Visitors.h"

Wall::Wall(Playground &playground)
    : GameObjectBase(playground) {
}

void Wall::visit(IObjectVisitor &visitor) {
	visitor(*this);
}

void Wall::visit(IConstObjectVisitor &visitor) const {
	visitor(*this);
}

void Wall::position(Position const &value) {
	position_ = value;
}

Position const &Wall::position() const {
	return position_;
}

TextureRef const &Wall::texture() const {
	return texture_;
}

void Wall::texture(TextureRef const &texture) {
	texture_ = texture;
}

void Wall::bonus(Bonus value) {
	bonus_ = value;
}

void Wall::destroyable(bool value) {
	this->destroyable_ = value;
}

void Wall::dropBonus() {
	if (bonus_ == Bonus::None) {
		return;
	}
	BonusItem &newBonusItem = playground_.createBonusItem(bonus_);
	newBonusItem.position(position_);
}

void Wall::destroyWall() {
	dropBonus();
	playground_.remove(*this);
}

bool Wall::destroyableGetter() {
	return destroyable_;
}