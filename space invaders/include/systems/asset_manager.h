#pragma once

#include "systems/sprite_renderer.h"

#include <map>
#include <string>
#include <vector>

class AssetManager {
private:
	static constexpr const char* letters_path_mask_ = "gfx/#let.png";
	static constexpr const char* digits_path_mask_ = "gfx/num#.png";
	static constexpr const char* obstacle_path_mask_ = "gfx/Obstacle#.png";
	static constexpr const size_t obstacle_sprites_count_ = 7;

	static constexpr const char* player_sprite_path_ = "gfx/Big Invader.png";
	static constexpr const char* enemy_sprite_path_ = "gfx/Little Invader.png";
	static constexpr const char* bullet_sprite_path_ = "gfx/bullet.png";
	static constexpr const char* ufo_sprite_path_ = "gfx/ufo.png";

	static constexpr char index_placeholder = '#';

	static constexpr const char* player_tag_ = "player";
	static constexpr const char* bullet_tag_ = "bullet";
	static constexpr const char* enemy_tag_ = "enemy";
	static constexpr const char* ufo_tag_ = "ufo";
	static constexpr const char* obstacle_tag_ = "obstacle";

	std::map<std::string, void*> sprites_;
	const SpriteRenderer& sprite_renderer_;

	void preload_letters();
	void preload_digits();
	void preload_obstacle_sprites();
public:
	AssetManager(const SpriteRenderer& sprite_renderer) : sprite_renderer_(sprite_renderer) {}

	void preload();

	void add_sprite(const std::string& name, const std::string& path);
	void* get_sprite(const std::string& tag) const;

	void* get_player_sprite() const;
	void* get_bullet_sprite() const;
	void* get_enemy_sprite() const;
	void* get_ufo_sprite() const;
	std::vector<void*> get_obstacle_sprites() const;
};