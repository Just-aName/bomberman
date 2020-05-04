#pragma once

#include <chrono>

class Config {
public:
	/**
	 * How long it takes for a player to move between two coarse cells.
	 */
	static std::chrono::milliseconds cellTime;

	/**
	 * The granularity of the fine grid.
	 * Game tick rate is cellTime/mesh.
	 */
	static const int mesh;

	/**
	 * Number of game ticks for bomb to explode.
	 */
	static const int bombTime;
	static const int fireTime;
	static const int poisonTime;
};
