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

	const Vec2 bullet_offset_{ 0.0f, 2.0f };
	const Vec2 bullet_size_{ 1.0f };
	const float bullet_angle_ = deg_to_rad(-90.0f);
	const Vec2 bullet_collision_offset_{ 0.0f, 1.5f };
	const float bullet_collision_size_ = 0.5f;
	const int bullet_life_span_ = 150;
public:
	static constexpr const char* bullet_tag = "e_bullet";

	BulletDir bullet_dir = BulletDir::DOWN;
	float bullet_speed = 60.0f;

	Vec2 trajectory_size{ 1.0f, 0.7f };
	float trajectory_speed = 0.05;

	void shoot(EntityManager& entity_manager, const FrameContext& frame_context, void* bullet_sprite);
	void update(const FrameContext& frame_context) override;
};