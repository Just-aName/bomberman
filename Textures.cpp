#include "Textures.h"
#include <SDL_render.h>
#include <stdexcept>

Textures::Textures(MySDL::Renderer &renderer)
    : renderer_(renderer) {
}

MySDL::Texture &Textures::get(TextureRef const &ref) {
	auto found = textures_.find(ref.name);
	if (found != textures_.end())
		return found->second;

	try {
		auto result = textures_.emplace(ref.name, loadTexture(renderer_, ref.name));
		return result.first->second;
	} catch (std::exception const &) {
		throw std::runtime_error(std::string("Texture not found: ") + ref.name);
	}
}
