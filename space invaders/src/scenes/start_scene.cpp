#pragma once 
#include "scenes/start_scene.h"
#include "utils/utility.h"

#include <cassert>

StartScene::StartScene(SceneType type, GameServices& services)
	: Scene(type, services) { setup_hud(); }

/// Creates HUD text elements and stores the changeable ones in game_hud_ under specific tags,
/// so they can be accessed and updated later. Static text that will not change does not use constant tags.
void StartScene::setup_hud() {
	auto header_text = GameText(
		services_.asset_manager, 
		CTransform(pos_to_px(Vec2(50, 10), services_.window_size), size_to_px(Vec2(3.5f), services_.window_size)),
		"space invaders");

	auto press_shift_text = GameText(
		services_.asset_manager,
		CTransform(pos_to_px(Vec2(50, 50), services_.window_size), size_to_px(Vec2(2.5f), services_.window_size)),
		"press shift to start");


	auto movement_instruction = GameText(
		services_.asset_manager, 
		CTransform(pos_to_px(Vec2(50, 80), services_.window_size), size_to_px(Vec2(1.2f), services_.window_size)),
		"use left and right arrow to move");

	auto shoot_instruction = GameText(
		services_.asset_manager,
		CTransform(pos_to_px(Vec2(50, 90), services_.window_size), size_to_px(Vec2(1.2f), services_.window_size)),
		"use space to shoot enemies");

	// we don't really need to remember header text tag
	game_hud_.add_text("header", header_text);
	game_hud_.add_text("movement_instruction", movement_instruction);
	game_hud_.add_text("shoot_instruction", shoot_instruction);

	game_hud_.add_text(press_shift_text_tag_, press_shift_text);
}

/// Draws game_hud_.
void StartScene::draw_entities() {
	game_hud_.draw_texts(services_.sprite_renderer);
}

/// Exits scene if 'shift' is pressed.
void StartScene::handle_input() {
	if (services_.input_handler.is_key_down(VK_SHIFT)) {
		end_scene();
	}
}

/// Updates text's bobbing.
void StartScene::update() {
	float bobbing_freq = 0.05f;
	float bobbing_scale = 10.0f;

	auto* text = game_hud_.get_text(press_shift_text_tag_);

	float pos_y = text->transform.pos.y;
	text->transform.pos.y = bobbing(clock_, pos_y, bobbing_freq, bobbing_scale);
}

void StartScene::clear() {
	game_hud_.clear();
}