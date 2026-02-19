#pragma once
#include "entities/entity.h"

#include <vector>

class Obstacle : public Entity {
protected:
	friend class EntityManager;
	Obstacle(const size_t id, const std::string& tag) : Entity(id, tag) {}

	std::vector<void*> sprites_;
	size_t sprites_count_ = 0;
	size_t sprite_ptr_ = 0;

public:
	void set_sprites(const std::vector<void*> sprites) { 
		sprites_count_ = sprites.size();
		sprites_ = sprites; 
	}

	/// Decreases the health of the obstacle and updates its current sprite
	void damage() {
		if (!has_component<CHealth>() || !has_component<CRenderer>()) return;

		get_component<CHealth>().current_hp--;

		if (sprite_ptr_ + 1 >= sprites_count_) return;
		get_component<CRenderer>().sprite = sprites_[++sprite_ptr_];
	}
};