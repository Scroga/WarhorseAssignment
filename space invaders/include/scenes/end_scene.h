#pragma once

#include "scene.h"
#include "utils/game_text.h"

class EndScene : public Scene {
private:
	const std::string PRESS_SHIFT_TEXT_TAG = "text";

public:
	EndScene(SceneType type, GameServices& services, int score, int best_score);

	void setup_hud() override;
	void draw_entities() override;
	void handle_input(const GameClock& clock) override;
	void update(const GameClock& clock) override;
	void clear() override;
}; 