#pragma once

#include "entities/entity.h"
#include "entities/bullet.h"
#include "systems/entity_manager.h"
#include "utils/utility.h"
#include "utils/vec2.h"

class Player : public Entity {
protected:
	friend class EntityManager;
	Player(const size_t id, const std::string& tag);
private:
	const DWORD NORMAL_COLOR = 0xffffffff;
	const DWORD DAMAGE_COLOR = 0xffdd1111; // red

	const Vec2 bullet_offset_{ 0.0f, -10.0f };
	const Vec2 bullet_size_{ 15.0f, 15.0f };
	const float bullet_angle_ = deg_to_rad(90.0f);
	const Vec2 bullet_collision_offset_{ 0.0f, -12.0f };
	const float bullet_collision_size_ = 6.0f;
	const float bullet_speed_ = 500.0f;

	const milliseconds damage_color_change_{ 500 };
	const milliseconds enemy_shoot_cooldown_{ 400 };

	GameTimer shoot_timer_;
	GameTimer color_chage_timer_;
public:
	static constexpr const char* bullet_tag = "p_bullet";

	BulletDir bullet_dir = BulletDir::UP;
	float movement_speed = 600.0f;

	float bobbing_freq = 0.02f;
	float bobbing_scale = 10.0f;

	void shoot(EntityManager& entity_manager, void* bullet_sprite);
	void move(MovementDir dir, const GameClock& clock, Vec2 window_size);
	void on_damage();
	void update(const GameClock& clock) override;
};