#pragma once

#include "Geometry.h"

struct SDL_Rect;

namespace MySDL {
class Renderer;
class Texture;
} // namespace MySDL

class Rectangle;
class Point;
class Vector;
class Position;
class Rgb;
class Dimensions;

/**
 * Graphical grid. Actually we have two grids,
 * one is coarse (and it is drawn) and the other is finer.
 */
class Grid {
public:
	/**
	 * @param renderer
	 * @param area - area of screen (in pixels) which is available to the grid
	 * @param dimensions - how many fields are there in the map
	 */
	Grid(MySDL::Renderer &renderer, Rectangle area, Dimensions const &dimensions);

	/**
	 * @brief Draws only the grid on a black background.
	 */
	void drawGrid();

	/**
	 * @brief Fills 'square' with 'rgb' color.
	 */
	void fill(Position square, Rgb rgb);

	/**
	 * @brief Draws 'texture' rotated by 'angle' to 'square'.
	 */
	void draw(Position square, MySDL::Texture &texture, double angle);

private:
	Vector physicalSize() const;
	Rectangle toGridCell(Position const &square);
	SDL_Rect toPhysical(Position const &square);
	void drawInnerGrid();
	void drawBorder();
	int logToPhy_x(int x) const;

	static int calcCellSize(Rectangle const &area, Dimensions const &dimensions);

	MySDL::Renderer &renderer_;
	Dimensions const &dimensions_;
	int const fineCellSize_;
	Vector const offset_;
};
