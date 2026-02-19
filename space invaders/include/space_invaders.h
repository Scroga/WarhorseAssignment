#pragma once
#include <memory>
#include <utility>
#include <algorithm>
#include <map>

#include "scenes/scene.h"
#include "utils/game_clock.h"
#include "utils/game_services.h"

class SpaceInvaders {
private:
	std::unique_ptr<Scene> current_scene_;
	GameServices& services_;
	GameClock game_clock_;
	int best_score_ = 0;

	void handle_scenes();
public:
	SpaceInvaders(GameServices& services);
	void in_loop();
};