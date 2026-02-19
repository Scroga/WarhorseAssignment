#include "systems/asset_manager.h"

#include <cassert>

void AssetManager::preload() {
	preload_letters();
	preload_digits();
	preload_obstacle_sprites();

	add_sprite(bullet_tag_, bullet_sprite_path_);
	add_sprite(player_tag_, player_sprite_path_);
	add_sprite(enemy_tag_, enemy_sprite_path_);
	add_sprite(ufo_tag_, ufo_sprite_path_);
}

/// Takes a tag and a sprite path and stores the sprite into the map
/// @param name Tag used to store and later retrieve the sprite
/// @param path Path to the sprite file to load and store
void AssetManager::add_sprite(const std::string& name, const std::string& path) {
	void* sprite = sprite_renderer_.load_sprite(path.c_str());
	assert(sprite && "load_sprite returned nullptr");
	sprites_[name] = sprite;
}

/// Returns the sprite stored under the specified tag
/// @param tag Map key used to retrieve the sprite
/// @return Pointer to the sprite if it exists; otherwise, nullptr
void* AssetManager::get_sprite(const std::string& tag) const {
	auto it = sprites_.find(tag);
	return (it == sprites_.end()) ? nullptr : it->second;
}

/// Uses letters_path_mask_ to construct the path for each letter sprite,
/// and stores each sprite under its corresponding tag
void AssetManager::preload_letters() {
	std::string path = letters_path_mask_;
	auto pos = path.find(index_placeholder);

	if (pos != std::string::npos) {
		for (char ch = 'a'; ch <= 'z'; ++ch) {
			path[pos] = ch;
			add_sprite(std::string(1, ch),path);
		}
	}
}

/// Uses digits_path_mask_ to construct the path for each digit sprite,
/// and stores each sprite under its corresponding tag
void AssetManager::preload_digits() {
	std::string path = digits_path_mask_;
	auto pos = path.find(index_placeholder);

	if (pos != std::string::npos) {
		for (char ch = '0'; ch <= '9'; ch++) {
			path[pos] = ch;
			add_sprite(std::string(1, ch), path);
		}
	}
}

/// Uses obstacle_path_mask_ and the obstacle_sprites_count_ to construct 
/// the path for each obstacle sprite, and stores each sprite under its corresponding tag.
void AssetManager::preload_obstacle_sprites() {
	std::string path = obstacle_path_mask_;
	auto pos = path.find(index_placeholder);

	if (pos != std::string::npos) {
		for (size_t i = 0; i < obstacle_sprites_count_; ++i) {
			char ch = '1' + static_cast<char>(i);
			path[pos] = ch;
			std::string key = std::string(obstacle_tag_) + ch;
			add_sprite(key, path);
		}
	}
}

void* AssetManager::get_player_sprite() const { return get_sprite(player_tag_); }
void* AssetManager::get_bullet_sprite() const { return get_sprite(bullet_tag_); }
void* AssetManager::get_enemy_sprite() const { return get_sprite(enemy_tag_); }
void* AssetManager::get_ufo_sprite() const { return get_sprite(ufo_tag_); }

/// Constructs and returns a vector of obstacle sprites
/// @return A vector of pointers to the obstacle sprites
std::vector<void*> AssetManager::get_obstacle_sprites() const {
	std::vector<void*> sprites;
	sprites.reserve(obstacle_sprites_count_);
	
	for (size_t i = 0; i < obstacle_sprites_count_; ++i) {
		char ch = '1' + static_cast<char>(i);
		std::string key = std::string(obstacle_tag_) + ch;
		void* sprite = get_sprite(key);
		sprites.push_back(sprite);
	}

	return sprites;
}