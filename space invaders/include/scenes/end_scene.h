#pragma once

#include "scene.h"
#include "utils/game_text.h"

class EndScene : public Scene {
private:
	const std::string press_shift_text_tag_ = "text";

public:
	EndScene(SceneType type, GameServices& services, int score, int best_score);

	void setup_hud() override;
	void draw_entities() override;
	void handle_input() override;
	void update() override;
	void clear() override;
}; 