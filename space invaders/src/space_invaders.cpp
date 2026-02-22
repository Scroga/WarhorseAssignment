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

/// Handles game scenes. All scene transition logic and hierarchy are implemented in this method.
/// The initial scene is StartScene, when it ends, PlayScene starts.
/// When PlayScene ends, the player's score is passed to EndScene and EndScene starts.
/// When EndScene ends, the PlayScene restarts.
void SpaceInvaders::handle_scenes() {
	if (current_scene_->has_ended()) {
		if (current_scene_->type == SceneType::Start || current_scene_->type == SceneType::End) {
			scene_transition<PlayScene>(SceneType::Play, services_);
		}
		else if (current_scene_->type == SceneType::Play) {
			int current_score = 0;
			if (auto* scene = dynamic_cast<PlayScene*>(current_scene_.get())) {
				current_score = scene->get_score();
				best_score_ = best_score_ > current_score ? best_score_ : current_score;
			}
			scene_transition<EndScene>(SceneType::End, services_, current_score, best_score_);
		}
	}
}

/// Updates the current scene clock, input, scene logic and possible scene translation.
/// Updates all entities via the entity manager and renders all entities in the current scene.
void SpaceInvaders::in_loop() {
	current_scene_->update_clock();
	current_scene_->handle_input();
	current_scene_->update();

	handle_scenes();
	services_.entity_manager.update();

	current_scene_->draw_entities();
}


 