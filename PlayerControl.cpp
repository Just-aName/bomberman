#include "Config.h"
#include "Player.h"

#include "PlayerControl.h"

void PlayerControl::tick() {
	if (!player_.isMoving()) {
		int const moveTicks = 1;
		if (isUp())
			player_.move(Direction::Up, moveTicks);
		else if (isDown()) {
			player_.move(Direction::Down, moveTicks);
		} else if (isLeft()) {
			player_.move(Direction::Left, moveTicks);
		} else if (isRight()) {
			player_.move(Direction::Right, moveTicks);
		}
	}
}

void PlayerControl::handle_sdl_event(SDL_Event event) {
	if (event.type == SDL_KEYDOWN && !event.key.repeat) {
		handle_keypress(event.key.keysym.sym);
	} else if (event.type == SDL_KEYUP) {
		handle_keyrelease(event.key.keysym.sym);
	}
}

bool *PlayerControl::toKeyState(SDL_Keycode keycode) {
	if (keycode == settings.keyDown)
		return &isDown_;
	if (keycode == settings.keyUp)
		return &isUp_;
	if (keycode == settings.keyLeft)
		return &isLeft_;
	if (keycode == settings.keyRight)
		return &isRight_;
	return nullptr;
}

void PlayerControl::handle_keypress(SDL_Keycode keycode) {
	if (keycode == settings.keyBomb) {
		player_.bomb();
		return;
	}
	bool *key = toKeyState(keycode);
	if (key)
		*key = true;
}

void PlayerControl::handle_keyrelease(SDL_Keycode keycode) {
	bool *key = toKeyState(keycode);
	if (key)
		*key = false;
}
