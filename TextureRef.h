#pragma once

#include <string>

class TextureRef {
public:
	std::string name;
};

inline bool operator==(TextureRef const &a, TextureRef const &b) {
	return a.name == b.name;
}
