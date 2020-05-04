#pragma once

#include "Geometry.h"

class Dimensions {
public:
	Dimensions(Vector coarseSize, int mesh);

	/**
	 * Size of the grid in coarse cells.
	 */
	Vector coarseSize() const {
		return coarseSize_;
	}

	int mesh() const {
		return mesh_;
	}

	/**
	 * Size of the grid in fine cells.
	 */
	Vector fineSize() const {
		return fineSize_;
	}

	/**
	 * Returns true iff the given square
	 * can be fully drawn inside the grid.
	 */
	bool isValid(Position const& square) const;

	Position coarsePosition(int x, int y) const;

private:
	Vector const coarseSize_;
	Vector const fineSize_;
	int const mesh_;
};
