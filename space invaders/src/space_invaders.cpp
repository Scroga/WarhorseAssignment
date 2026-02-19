#include "space_invaders.h"

#include "systems/asset_manager.h"
#include "systems/entity_manager.h"

#include "scenes/start_scene.h"
#include "scenes/play_scene.h"
#include "scenes/end_scene.h"
#include "scenes/scene_type.h"

SpaceInvaders::SpaceInvaders(GameServices& services) : services_(services)
{
	current_scene_ = std::make_unique<StartScene>(SceneType::Start, services_);
}

/// Handles game scenes. All scene transition logic and hierarchy are implemented in this method
/// The initial scene is StartScene; when it ends, PlayScene starts
/// When PlayScene ends, the player's score is passed to EndScene
/// When EndScene ends, the game restarts
void SpaceInvaders::handle_scenes() {
	if (current_scene_->has_ended()) {
		if (current_scene_->type == SceneType::Start || current_scene_->type == SceneType::End) {
			current_scene_->clear();
			services_.entity_manager.clear();
			current_scene_ = std::make_unique<PlayScene>(SceneType::Play, services_);
		}
		else if (current_scene_->type == SceneType::Play) {
			int current_score = 0;
			if (auto* scene = dynamic_cast<PlayScene*>(current_scene_.get())) {
				current_score = scene->get_score();
				best_score_ = best_score_ > current_score ? best_score_ : current_score;
			}
			current_scene_->clear();
			services_.entity_manager.clear();
			current_scene_ = std::make_unique<EndScene>(SceneType::End, services_, current_score, best_score_);
		}
	}
}

/// Updates the game_clock_, handles scene management,
/// updates the current scene, and draws the scene.
void SpaceInvaders::in_loop() {
	game_clock_.in_loop();

	current_scene_->handle_input(game_clock_);
	current_scene_->update(game_clock_);
	handle_scenes();

	services_.entity_manager.update();

	current_scene_->draw_entities();
}


 