#pragma once

class Point {
public:
	int x;
	int y;
};

bool operator==(Point const &a, Point const &b);
bool operator!=(Point const &a, Point const &b);

class Vector {
public:
	int x;
	int y;
};

Vector one(Vector const &vector);

bool operator==(Vector const &a, Vector const &b);
bool operator!=(Vector const &a, Vector const &b);
Vector &operator+=(Vector &vec1, Vector const &vec2);
Vector operator+(Vector const &vec1, Vector const &vec2);
Vector &operator-=(Vector &vec1, Vector const &vec2);
Vector operator-(Vector const &vec1, Vector const &vec2);
Vector &operator*=(Vector &vec, int m);
Vector operator*(Vector const &vec, int m);
Vector operator*(int m, Vector const &vec);
Vector &operator/=(Vector &vec, int m);
Vector operator/(Vector const &vec, int m);

class Rectangle {
public:
	Point from; // topleft
	Point to; // bottomright

	Vector size() const;
};

class Position {
public:
	Point from; // topleft
	int size;
};

bool operator==(Position const &a, Position const &b);
bool operator!=(Position const &a, Position const &b);

Position round_x(Position const &square);
Position round_y(Position const &square);
Position round(Position const &square);

bool isCentered(Position const &square);

/**
 * @returns true iff the two squares overlap at least partially
 */
bool overlap(Position const &a, Position const &b);

Point &operator+=(Point &point, Vector const &vector);
Point operator+(Point const &point, Vector const &vector);
Vector operator-(Point const &a, Point const &b);
Position &operator+=(Position &square, Vector const &vector);
Position operator+(Position const &square, Vector const &vector);

enum class Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

Direction opposite(Direction direction);

Vector toVector(Direction direction);

Position next(Position const &square, Vector direction, int n = 1);
Position next(Position const &square, Direction direction, int n = 1);
