#pragma once

#include "scene.h"
#include "utils/game_text.h"

class StartScene : public Scene{
private:
	const std::string PRESS_SHIFT_TEXT_TAG = "text";
public:
	StartScene(SceneType type, GameServices& services);

	void setup_hud() override;
	void draw_entities() override;
	void handle_input(const GameClock& clock) override;
	void update(const GameClock& clock) override;
	void clear() override;
};