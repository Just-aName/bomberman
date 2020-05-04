#pragma once

#include <SDL_events.h>

class Player;

class PlayerControl {
public:
	explicit PlayerControl(Player &player)
	    : player_(player) {}

	struct Settings {
		SDL_Keycode keyLeft;
		SDL_Keycode keyDown;
		SDL_Keycode keyRight;
		SDL_Keycode keyUp;
		SDL_Keycode keyBomb;
	};

	Settings settings{};

	void tick();
	void handle_sdl_event(SDL_Event event);

	bool isUp() const { return isUp_; }
	bool isDown() const { return isDown_; }
	bool isLeft() const { return isLeft_; }
	bool isRight() const { return isRight_; }

private:
	void handle_keypress(SDL_Keycode keycode);
	void handle_keyrelease(SDL_Keycode keycode);
	bool *toKeyState(SDL_Keycode keycode);

	Player &player_;
	bool isUp_ = false;
	bool isDown_ = false;
	bool isLeft_ = false;
	bool isRight_ = false;
};
