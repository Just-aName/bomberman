#pragma once

#include "Geometry.h"
#include <ostream>

inline std::ostream &operator<<(std::ostream &o, Point const &point) {
	o << "[" << point.x << "," << point.y << "]";
	return o;
}

inline std::ostream &operator<<(std::ostream &o, Rectangle const &rect) {
	o << "{" << rect.from << " -> " << rect.to << "}";
	return o;
}

inline std::ostream &operator<<(std::ostream &o, Position const &square) {
	Rectangle rect{ square.from, square.from + Vector{ square.size, square.size } };
	o << rect;
	return o;
}
