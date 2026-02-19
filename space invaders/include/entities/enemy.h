#pragma once

#include "utils/utility.h"
#include "systems/entity_manager.h"
#include "entities/entity.h"
#include "entities/bullet.h"

class Enemy : public Entity {
protected:
	friend class EntityManager;
	Enemy(const size_t id, const std::string& tag);
private:
	const DWORD default_bullet_color_ = 0xffffeeee;

	const Vec2 bullet_offset_{ 0.0f, 10.0f };
	const Vec2 bullet_size_{ 15.0f, 15.0f };
	const float bullet_angle_ = deg_to_rad(-90.0f);
	const Vec2 bullet_collision_offset_{ 0.0f, 12.0f };
	const float bullet_collision_size_ = 6.0f;
	const int bullet_life_span_ = 200;
public:
	static constexpr const char* bullet_tag = "e_bullet";

	BulletDir bullet_dir = BulletDir::DOWN;
	float bullet_speed = 200.0f;

	float bobbing_freq = 0.01f;
	float bobbing_scale = -35.0f;

	void shoot(EntityManager& entity_manager, void* bullet_sprite);
	void update(const GameClock& clock) override;
};