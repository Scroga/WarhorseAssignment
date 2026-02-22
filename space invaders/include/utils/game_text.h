#pragma once

#include "utils/components.h"
#include "systems/sprite_renderer.h"
#include "systems/asset_manager.h"
#include "utils/vec2.h"

#include <vector>
#include <sstream>
#include <cassert>

class GameText {
private:
	AssetManager& asset_manager_;
	std::vector<void*> letter_sprites_;
public:
	CTransform transform;
	float text_margin = 1.5f;

	GameText() = default;
	GameText(AssetManager& asset_manager, CTransform t, const std::string& text) 
		: asset_manager_(asset_manager), transform(t) {
		set_text(text);
	}

	GameText(AssetManager& asset_manager, CTransform t, int number) 
		: asset_manager_(asset_manager), transform(t) {
		set_text(number);
	}

	void set_text(const std::string& text) {
		letter_sprites_.clear();
		letter_sprites_.reserve(text.size());

		for (char c : text) {
			std::string key(1, c);
			letter_sprites_.push_back(asset_manager_.get_sprite(key));
		}
	}

	void set_text(int number) {
		std::stringstream ss;
		ss << number;
		set_text(ss.str());
	}

	/// Draws text by rendering each letter sprite next to the previouse one.
	/// The text is horizontally centred around transfrom.pos.x.
	/// Space between letters is controlled by 'text_margin'.
	/// @pram renderer Sprite renderer is used to draw individual letter sprites.
	void draw(const SpriteRenderer& renderer) {
		float text_length = transform.size.x * text_margin * (letter_sprites_.size() - 1);
		float spawn_pos_x = transform.pos.x - text_length * 0.5f;

		for (size_t i = 0; i < letter_sprites_.size(); i++) {
			float pos_x = spawn_pos_x + (transform.size.x * text_margin * i);
			void* sprite = letter_sprites_[i];
			
			if (!sprite) continue;

			renderer.draw_sprite(
				sprite,
				pos_x, transform.pos.y,
				transform.size.x, transform.size.y,
				transform.angle);
		}
	}
};