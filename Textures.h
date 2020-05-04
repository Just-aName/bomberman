#pragma once

#include "SDLWrapper.h"
#include "TextureRef.h"
#include <map>

class Textures {
public:
	explicit Textures(MySDL::Renderer& renderer);

	MySDL::Texture& get(TextureRef const& ref);

private:
	MySDL::Renderer& renderer_;
	std::map<std::string, MySDL::Texture> textures_;
};
