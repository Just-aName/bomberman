#include <algorithm> // min, max
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include <SDL.h>

#include "clara.hpp"

#include "Bomberman.h"
#include "Config.h"
#include "Dimensions.h"
#include "Grid.h"
#include "Player.h"
#include "PlayerControl.h"
#include "Playground.h"
#include "SDLWrapper.h"
#include "Textures.h"
#include "Visitors.h"
#include "Wall.h"

using namespace std;

static constexpr int SCREEN_WIDTH = 640;
static constexpr int SCREEN_HEIGHT = 480;

class App {
public:
	explicit App(std::string const &gameMap);
	App(App const &) = delete;
	App &operator=(App const &) = delete;

	void main();

private:
	void update();
	void render();
	void handle_sdl_events();
	void renderMapItems();
	void renderGameOverText();
	void playerControl();

	MySDL::SDL sdl{ SDL_INIT_VIDEO };
	MySDL::Window window{ "Bomberman", SCREEN_WIDTH, SCREEN_HEIGHT };
	MySDL::Renderer renderer{ window };

	Bomberman bomberman;

	Grid grid{ renderer, { { 0, 0 }, { SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 } }, bomberman.dimensions };
	Textures textures{ renderer };
	MySDL::Texture gameOver = loadTexture(renderer, "gameOver.png");

#ifndef FACTORY_NOT_IMPLEMENTED
	PlayerControl playerControl1{ bomberman.player1 };
	PlayerControl playerControl2{ bomberman.player2 };
#endif

	bool stop = false;
	chrono::steady_clock::time_point next_update{};
	chrono::steady_clock::time_point next_render{};
};

App::App(std::string const &gameMap)
    : bomberman(gameMap) {
#ifndef FACTORY_NOT_IMPLEMENTED
	bomberman.player2.texture({ "troll.png" });

	playerControl1.settings.keyLeft = SDLK_LEFT;
	playerControl1.settings.keyRight = SDLK_RIGHT;
	playerControl1.settings.keyUp = SDLK_UP;
	playerControl1.settings.keyDown = SDLK_DOWN;
	playerControl1.settings.keyBomb = SDLK_RETURN;

	playerControl2.settings.keyLeft = SDLK_a;
	playerControl2.settings.keyRight = SDLK_d;
	playerControl2.settings.keyUp = SDLK_w;
	playerControl2.settings.keyDown = SDLK_s;
	playerControl2.settings.keyBomb = SDLK_SPACE;
#endif
}

void App::main() {
	const auto update_dt = Config::cellTime / Config::mesh;
	const auto render_dt = 16ms;
	next_update = next_render = chrono::steady_clock::now();

	while (!stop) {
		auto currentTime = chrono::steady_clock::now();

		if (currentTime >= next_update) {
			next_update += update_dt;
			update();
		}

		if (currentTime >= next_render) {
			next_render += render_dt;
			handle_sdl_events();
			render();
		}

		std::this_thread::sleep_until(std::min(next_render, next_update));
	}
}

void App::handle_sdl_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (!bomberman.playground.gameStopped()) {
#ifndef FACTORY_NOT_IMPLEMENTED
			playerControl1.handle_sdl_event(e);
			playerControl2.handle_sdl_event(e);
#endif
		}
		if (e.type == SDL_QUIT) {
			stop = true;
		} else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) {
			stop = true;
		}
	}
}

void App::render() {
	renderer.setColor(20, 20, 40, SDL_ALPHA_OPAQUE);
	renderer.clear();

	grid.drawGrid();
	renderMapItems();
	renderGameOverText();

	// flip the backbuffer
	// this means that everything that we prepared behind the screens is actually shown
	renderer.render();
}

double fit(Vector const &inner, Vector const &outer) {
	double const fitx = double(outer.x) / double(inner.x);
	double const fity = double(outer.y) / double(inner.y);
	return std::min(fitx, fity);
}

void App::renderGameOverText() {
	if (!bomberman.playground.gameStopped())
		return;

	MySDL::Texture &texture = gameOver;
	SDL_SetTextureAlphaMod(texture.get(), 200);

	Vector const winSize = window.size();
	Vector const textSize = texture.size();
	double const ratio = fit(textSize, winSize);
	Vector const resultSize = int(ratio) * textSize;
	Rectangle box;
	box.from = Point{ 0, 0 } + (winSize - resultSize) / 2;
	box.to = box.from + resultSize;
	draw(renderer, texture, box, 0);
}

/**
 * @brief Represents a logical step in the game.
 */
void App::update() {
	playerControl();
	bomberman.playground.gameTick();
}

void App::playerControl() {
	if (!bomberman.playground.gameStopped()) {
#ifndef FACTORY_NOT_IMPLEMENTED
		playerControl1.tick();
		playerControl2.tick();
#endif
	}
}

void App::renderMapItems() {
	bomberman.playground.forEach([&](IGameObject &object) {
		DeadPlayerDetector detector;
		object.visit(detector);
		grid.draw(object.position(), textures.get(object.texture()), detector.detected ? 80 : 0);
	});
#ifdef FACTORY_NOT_IMPLEMENTED
	auto center = bomberman.dimensions.coarseSize() / 2;
	grid.draw(bomberman.dimensions.coarsePosition(center.x, center.y), textures.get({ "human.png" }), 0);
#endif
}

struct Options {
	bool help = false;
	string gameMapFile{ "maps/default.txt" };
	unsigned int cellTimeMs = 250;
};

Options cmdOpts(int argc, char **argv) {
	using namespace clara;
	Options options;

	int width;
	auto cli = Help(options.help)
	    | Opt(width, "width")["-w"]["--width"]("How wide should it be?")
	    | Opt(options.cellTimeMs, "time [ms]")["--celltime"]("Time it takes Player to move between cells (in milliseconds)")
	    | Opt(options.gameMapFile, "map")["-m"]["--map"]("Path to map");

	auto result = cli.parse(Args(argc, argv));
	if (!result) {
		std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
		exit(1);
	}

	if (options.help) {
		std::cout << cli;
		exit(0);
	}

	return options;
}

// https://stackoverflow.com/a/116220/6209703
string slurp(string const &filename) {
	ifstream is{ filename };
	if (!is.is_open()) {
		cerr << "Cannot open file '" << filename << "'." << endl;
		exit(1);
	}
	stringstream sstr;
	sstr << is.rdbuf();
	return sstr.str();
}

int main(int argc, char *argv[]) {
	Options options = cmdOpts(argc, argv);
	Config::cellTime = std::chrono::milliseconds(options.cellTimeMs);
	App app{ slurp(options.gameMapFile) };
	app.main();

	return 0;
}
