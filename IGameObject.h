#pragma once

class Position;

class TextureRef;
class IObjectVisitor;
class IConstObjectVisitor;

/**
 * Interface for all game objects found on the playground.
 * The object has a position, can be drawn (using the texture)
 * and performs certain actions every game tick.
 */
class IGameObject {

public:
	
	virtual ~IGameObject() = default;

	virtual void visit(IObjectVisitor &visitor) = 0;
	virtual void visit(IConstObjectVisitor &visitor) const = 0;

	virtual void gameTick() = 0;

	virtual void position(Position const &value) = 0;
	virtual Position const &position() const = 0;

	virtual TextureRef const &texture() const = 0;
	virtual void texture(TextureRef const &texture) = 0;
};
