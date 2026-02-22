#pragma once
#include <memory>
#include <utility>
#include <algorithm>
#include <type_traits>
#include <map>

#include "scenes/scene.h"
#include "utils/game_clock.h"
#include "utils/game_services.h"

class SpaceInvaders {
private:
	std::unique_ptr<Scene> current_scene_;
	GameServices& services_;
	int best_score_ = 0;

	void handle_scenes();

	template<typename TScene, typename... TArgs,
		typename = std::enable_if_t<std::is_base_of<Scene, TScene>::value>>
	void scene_transition(TArgs&&... args) {
		current_scene_->clear();
		services_.entity_manager.clear();
		current_scene_ = std::make_unique<TScene>(std::forward<TArgs>(args)...);
	}

public:
	SpaceInvaders(GameServices& services);
	void in_loop();
};