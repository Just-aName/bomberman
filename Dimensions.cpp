#include "Dimensions.h"

Dimensions::Dimensions(Vector coarseSize, int mesh)
    : coarseSize_(coarseSize)
    , fineSize_(mesh * coarseSize)
    , mesh_(mesh) {
}

bool Dimensions::isValid(Position const& square) const {
	return square.from.x >= 0
	    && square.from.x + square.size <= fineSize().x
	    && square.from.y >= 0
	    && square.from.y + square.size <= fineSize().y;
}

Position Dimensions::coarsePosition(int x, int y) const {
	return Position{ Point{ x * mesh_, y * mesh_ }, mesh_ };
}
