#pragma once 
#include "scenes/start_scene.h"
#include "utils/utility.h"

#include <cassert>

StartScene::StartScene(SceneType type, GameServices& services)
	: Scene(type, services) { setup_hud(); }

// Creates HUD text elements and stores the changeable ones in game_hud_ under specific tags,
// so they can be accessed and updated later. Static text that will not change does not use constant tags
void StartScene::setup_hud() {
	Vec2 header_text_pos{ window_size_.x / 2.0f, window_size_.y / 12.0f };
	auto header_text = GameText(services_.asset_manager, CTransform(header_text_pos, Vec2{ 40.0f }), "space invaders");

	Vec2 press_shift_text_pos{ window_size_.x / 2.0f, window_size_.y / 2.0f };
	auto press_shift_text = GameText(services_.asset_manager, CTransform(press_shift_text_pos, Vec2{ 30.0f }), "press shift to start");

	Vec2 movement_instruction_pos{ window_size_.x / 2.0f, window_size_.y - window_size_.y / 7.0f };
	auto movement_instruction = GameText(services_.asset_manager, CTransform(movement_instruction_pos, Vec2{ 15.0f }), "use left or right arrow to move");

	Vec2 shoot_instruction_pos{ window_size_.x / 2.2f, window_size_.y - window_size_.y / 12.0f };
	auto shoot_instruction = GameText(services_.asset_manager, CTransform(shoot_instruction_pos, Vec2{ 15.0f }), "use space to shoot enemies");

	// we don't really need to remember header text tag
	game_hud_.add_text("header", header_text);
	game_hud_.add_text("movement_instruction", movement_instruction);
	game_hud_.add_text("shoot_instruction", shoot_instruction);

	game_hud_.add_text(PRESS_SHIFT_TEXT_TAG, press_shift_text);
}

void StartScene::draw_entities() {
	game_hud_.draw(services_.sprite_renderer);
}

void StartScene::handle_input(const GameClock& clock) {
	if (services_.input_handler.is_key_down(VK_SHIFT)) {
		end_scene();
	}
}

void StartScene::update(const GameClock& clock) {
	float bobbing_freq = 0.05f;
	float bobbing_scale = 10.0f;

	auto* text = game_hud_.get_text(PRESS_SHIFT_TEXT_TAG);

	float pos_y = text->transform.pos.y;
	text->transform.pos.y = bobbing(clock, pos_y, bobbing_freq, bobbing_scale);
}

void StartScene::clear() {
	game_hud_.clear();
}