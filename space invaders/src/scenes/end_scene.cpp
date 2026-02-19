#include "scenes/end_scene.h"
#include "utils/utility.h"

#include <sstream>

EndScene::EndScene(SceneType type, GameServices& services, int score, int best_score)
	: Scene(type, services) {
	setup_hud();

	// Create static texts
	auto current_score_text = GameText(
		services_.asset_manager, 
		CTransform(pos_to_px(Vec2(75, 15), services_.window_size), size_to_px(Vec2(2.5f), services_.window_size)),
		score);

	auto best_score_text = GameText(
		services_.asset_manager,
		CTransform(pos_to_px(Vec2(75, 30), services_.window_size), size_to_px(Vec2(2.5f), services_.window_size)),
		best_score);

	// we will not update these texts, so we don't need to remember their tags.
	game_hud_.add_text("current_score", current_score_text);
	game_hud_.add_text("best_score", best_score_text);
}


/// Creates HUD text elements and stores the changeable ones in game_hud_ under specific tags,
/// so they can be accessed and updated later. Static text that will not change does not use constant tags
void EndScene::setup_hud() {
	auto press_shift_text = GameText(
		services_.asset_manager,
		CTransform(pos_to_px(Vec2(50, 75), services_.window_size), size_to_px(Vec2(2.5f), services_.window_size)),
		"press shift to restart");

	auto current_score_text = GameText(
		services_.asset_manager,
		CTransform(pos_to_px(Vec2(30, 15), services_.window_size), size_to_px(Vec2(2.0f), services_.window_size)),
		"your current score ");

	auto best_score_text = GameText(
		services_.asset_manager,
		CTransform(pos_to_px(Vec2(30, 30), services_.window_size), size_to_px(Vec2(2.0f), services_.window_size)),
		"your  best   score ");

	// we will not update these texts, so we don't need to remember their tags.
	game_hud_.add_text("current_score_t", current_score_text);
	game_hud_.add_text("best_score_t", best_score_text);

	game_hud_.add_text(PRESS_SHIFT_TEXT_TAG, press_shift_text);
}

void EndScene::draw_entities() {
	game_hud_.draw(services_.sprite_renderer);
}

void EndScene::handle_input(const GameClock& clock) {
	if (services_.input_handler.is_key_down(VK_SHIFT)) {
		end_scene();
	}
}

void EndScene::update(const GameClock& clock) {
	float bobbing_freq = 0.05f;
	float bobbing_scale = 10.0f;

	auto* text = game_hud_.get_text(PRESS_SHIFT_TEXT_TAG);

	float pos_y = text->transform.pos.y;
	text->transform.pos.y = bobbing(clock, pos_y, bobbing_freq, bobbing_scale);
}

void EndScene::clear() {
	game_hud_.clear();
}