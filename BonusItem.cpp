#include "BonusItem.h"
#include "Visitors.h"

BonusItem::BonusItem(Playground &playground)
    : GameObjectBase(playground) {
}

void BonusItem::visit(IObjectVisitor &visitor) {
	visitor(*this);
}

void BonusItem::visit(IConstObjectVisitor &visitor) const {
	visitor(*this);
}

void BonusItem::position(Position const &value) {
	position_ = value;
}

Position const &BonusItem::position() const {
	return position_;
}

TextureRef const &BonusItem::texture() const {
	return texture_;
}

void BonusItem::texture(TextureRef const &texture) {
	texture_ = texture;
}

void BonusItem::bonus(Bonus value) {
	bonus_ = value;
}

void BonusItem::remove() {
	playground_.remove(*this);
}

const Bonus& BonusItem::bonusGetter() const {
    return bonus_;
}

void BonusItem::gameTick() {
	InFireChecker visitor(position_);
	playground_.visitAll(visitor);
	if (visitor.inFire) {
		remove();
	}
}