#include <SDL_render.h>

#include <algorithm>

#include "Dimensions.h"
#include "Geometry.h"
#include "Grid.h"
#include "SDLWrapper.h"

Grid::Grid(MySDL::Renderer &renderer, Rectangle area, Dimensions const &dimensions)
    : renderer_(renderer)
    , dimensions_(dimensions)
    , fineCellSize_(calcCellSize(area, dimensions))
    , offset_(((area.to - area.from) - physicalSize()) / 2) {}

int Grid::calcCellSize(Rectangle const &area, Dimensions const &dimensions) {
	// Border included :)
	Vector const size = area.to - area.from - Vector{ 1, 1 };
	Vector const fineGridSize = dimensions.fineSize();
	Vector const rectCellSize = { size.x / fineGridSize.x, size.y / fineGridSize.y };
	int const smaller = std::min(rectCellSize.x, rectCellSize.y);
	return smaller - 1;
}

Vector Grid::physicalSize() const {
	return (fineCellSize_ * dimensions_.mesh() + 1) * dimensions_.coarseSize();
}

void Grid::draw(Position square, MySDL::Texture &texture, double angle) {
	Rectangle const cell = toGridCell(square);
	::draw(renderer_, texture, cell, angle);
}

void Grid::fill(Position square, Rgb rgb) {
	SDL_Rect rect = toPhysical(square);
	renderer_.setColor(rgb);
	SDL_RenderFillRect(renderer_.get(), &rect);
}

void Grid::drawInnerGrid() {
	Vector const sz = offset_ + physicalSize();

	// X
	renderer_.setColor(255, 0, 0, 255);
	for (int i = 1; i < dimensions_.coarseSize().x; i++) {
		int x = offset_.x + i * (dimensions_.mesh() * fineCellSize_ + 1);
		renderer_.drawLine(x, offset_.y, x, sz.y - 1);
	}

	// Y
	for (int i = 1; i < dimensions_.coarseSize().y; i++) {
		int y = offset_.y + i * (dimensions_.mesh() * fineCellSize_ + 1);
		renderer_.drawLine(offset_.x, y, sz.x - 1, y);
	}
}

void Grid::drawGrid() {
	drawInnerGrid();
	drawBorder();
}

Rectangle Grid::toGridCell(Position const &square) {
	Vector const fineFrom = square.from - Point{ 0, 0 };
	Vector const coarseFrom = fineFrom / dimensions_.mesh();
	Vector const fineTo = fineFrom + Vector{ square.size, square.size };
	Vector const coarseTo = fineTo / dimensions_.mesh();

	// All the stuff in between
	Vector const realFineFrom = offset_ + fineCellSize_ * fineFrom + coarseFrom;
	Vector const realFineTo = offset_ + fineCellSize_ * fineTo + coarseTo;
	Rectangle const rect{ Point{ 1, 1 } + realFineFrom, Point{ 0, 0 } + realFineTo };
	return rect;
}

SDL_Rect Grid::toPhysical(Position const &square) {
	Rectangle const cell = toGridCell(square);
	return toSDL(cell);
}

void Grid::drawBorder() {
	Vector const sz = offset_ + physicalSize();
	renderer_.setColor(255, 0, 0, 255);
	renderer_.drawLine(offset_.x, offset_.y, offset_.x, sz.y);
	renderer_.drawLine(offset_.x, offset_.y, sz.x, offset_.y);
	renderer_.drawLine(sz.x, offset_.y, sz.x, sz.y);
	renderer_.drawLine(offset_.x, sz.y, sz.x, sz.y);
}
