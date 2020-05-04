#include <cassert>

#include "Geometry.h"

bool operator==(Point const &a, Point const &b) {
	return a.x == b.x && a.y == b.y;
}

bool operator!=(Point const &a, Point const &b) {
	return !(a == b);
}

bool operator==(Position const &a, Position const &b) {
	return a.from == b.from && a.size == b.size;
}

bool operator!=(Position const &a, Position const &b) {
	return !(a == b);
}

namespace {
Vector onBoth(Vector const &vector, int (*f)(int)) {
	return { f(vector.x), f(vector.y) };
}
} // namespace

bool overlap(Position const &a, Position const &b) {
	if (a.from.x < b.from.x && a.from.y < b.from.y) {
		return a.from.x + a.size > b.from.x && a.from.y + a.size > b.from.y;
	}

	if (a.from.x < b.from.x && a.from.y >= b.from.y) {
		return a.from.x + a.size > b.from.x && b.from.y + b.size > a.from.y;
	}

	if (a.from.x >= b.from.x && a.from.y >= b.from.y) {
		return b.from.x + b.size > a.from.x && b.from.y + b.size > a.from.y;
	}

	assert(a.from.x >= b.from.x && a.from.y < b.from.y);
	return b.from.x + b.size > a.from.x && a.from.y + a.size > b.from.y;
}

Vector one(Vector const &vector) {
	return onBoth(vector, [](int x) {
		if (x > 0)
			return +1;
		if (x < 0)
			return -1;
		return 0;
	});
}

bool operator==(Vector const &a, Vector const &b) {
	return a.x == b.x && a.y == b.y;
}

bool operator!=(Vector const &a, Vector const &b) {
	return !(a == b);
}

Vector &operator+=(Vector &vec1, Vector const &vec2) {
	vec1.x += vec2.x;
	vec1.y += vec2.y;
	return vec1;
}

Vector operator+(Vector const &vec1, Vector const &vec2) {
	Vector vec = vec1;
	vec += vec2;
	return vec;
}

Vector &operator-=(Vector &vec1, Vector const &vec2) {
	vec1.x -= vec2.x;
	vec1.y -= vec2.y;
	return vec1;
}

Vector operator-(Vector const &vec1, Vector const &vec2) {
	Vector vec = vec1;
	vec -= vec2;
	return vec;
}

Vector &operator*=(Vector &vec, int m) {
	vec.x *= m;
	vec.y *= m;
	return vec;
}

Vector operator*(Vector const &vec, int m) {
	Vector v = vec;
	v *= m;
	return v;
}

Vector operator*(int m, Vector const &vec) {
	return vec * m;
}

Vector &operator/=(Vector &vec, int m) {
	vec.x /= m;
	vec.y /= m;
	return vec;
}

Vector operator/(Vector const &vec, int m) {
	Vector v = vec;
	v /= m;
	return v;
}

Position round_x(Position const &square) {
	int const rx = square.from.x % square.size;
	int const x = square.from.x / square.size * square.size + (rx > square.size / 2 ? square.size : 0);
	return { { x, square.from.y }, square.size };
}

Position round_y(Position const &square) {
	int const ry = square.from.y % square.size;
	int const y = square.from.y / square.size * square.size + (ry > square.size / 2 ? square.size : 0);
	return { { square.from.x, y }, square.size };
}

Position round(Position const &square) {
	return round_y(round_x(square));
}

Point &operator+=(Point &point, Vector const &vector) {
	point.x += vector.x;
	point.y += vector.y;
	return point;
}

Point operator+(Point const &point, Vector const &vector) {
	Point p = point;
	p += vector;
	return p;
}

Vector operator-(Point const &a, Point const &b) {
	return { a.x - b.x, a.y - b.y };
}

Position &operator+=(Position &square, Vector const &vector) {
	square.from += vector;
	return square;
}

Position operator+(Position const &square, Vector const &vector) {
	Position s = square;
	s += vector;
	return s;
}

Vector Rectangle::size() const {
	return to - from;
}

bool isCentered(Position const &square) {
	return square == round(square);
}

Direction opposite(Direction direction) {
	switch (direction) {
	case Direction::Down:
		return Direction::Up;
	case Direction::Left:
		return Direction::Right;
	case Direction::Up:
		return Direction::Down;
	case Direction::Right:
		return Direction::Left;
	case Direction::None:
		return Direction::None;
	default:
		assert(false);
		return direction;
	}
}

Vector toVector(Direction direction) {
	switch (direction) {
	case Direction::Up:
		return { 0, -1 };
	case Direction::Down:
		return { 0, +1 };
	case Direction::Left:
		return { -1, 0 };
	case Direction::Right:
		return { +1, 0 };
	case Direction::None:
		return { 0, 0 };
	default:
		assert(false);
		return { 0, 0 };
	}
}

Position next(Position const &square, Vector direction, int n) {
	return square + square.size * n * direction;
}

Position next(Position const &square, Direction direction, int n) {
	return next(square, toVector(direction), n);
}
