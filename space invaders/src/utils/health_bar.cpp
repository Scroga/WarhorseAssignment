#include "utils/health_bar.h"

HealthBar::HealthBar(EntityManager& entity_manager, Vec2 pos, Vec2 s, void* spr, size_t p)
	: entity_manager_(entity_manager)
	, pos_(pos), size_(s), sprite_(spr)
	, points_(p), points_ptr_(p ? (p - 1) : 0) {
	reset();
}

void HealthBar::reset() {
	sprites_.clear();
	sprites_.reserve(points_);
	points_ptr_ = points_ ? (points_ - 1) : 0;

	float sprites_length_x = size_.x * margin * points_;
	float start_pos_x = pos_.x - sprites_length_x * 0.5f;

	for (size_t i = 0; i < points_; i++) {
		float pos_x = start_pos_x + size_.x * margin * i;
		auto sprite_entity = entity_manager_.spawn_sprite<Entity>(
			Vec2{ pos_x, pos_.y }, size_, deg_to_rad(0.0f),
			"health", sprite_);

		sprites_.push_back(std::move(sprite_entity));
	}
}

void HealthBar::increase() {
	points_ptr_ = (points_ptr_ == points_ - 1) ? points_ptr_ : points_ptr_ + 1;

	if (auto sprite = sprites_[points_ptr_].lock()) {
		auto& renderer = sprite->get_component<CRenderer>();
		renderer.color = DEFAULT_COLOR;
	}
}

void HealthBar::decrease() {
	if (auto sprite = sprites_[points_ptr_].lock()) {
		auto& renderer = sprite->get_component<CRenderer>();
		renderer.color = DAMAGE_COLOR;
	}

	points_ptr_ = (points_ptr_ == 0) ? points_ptr_ : points_ptr_ - 1;
}

void HealthBar::clear() { sprites_.clear(); }