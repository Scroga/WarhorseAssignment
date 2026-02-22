#include "entities/enemy.h"
#include "systems/asset_manager.h"
#include "systems/entity_manager.h"

Enemy::Enemy(const size_t id, const std::string& tag) : Entity(id, tag) {}

/// Spawns an enemy bullet and initializes its rendering, lifetime, and movement.
/// @param entity_manager Entity manager used to spawn the bullet entity.
/// @param frame_context Per-frame context (used for current window size).
/// @param bullet_sprite Sprite handle used to render the bullet.
void Enemy::shoot(EntityManager& entity_manager, const FrameContext& frame_context, void* bullet_sprite) {
	if (!has_component<CTransform>()) return;
	auto& transform = get_component<CTransform>();

	// Spawn and setup bullet
	auto bullet = entity_manager.spawn_actor<Bullet>(
		px_to_pos(transform.pos, frame_context.window_size) + bullet_offset_,
		bullet_size_, bullet_angle_,
		bullet_collision_offset_, bullet_collision_size_,
		bullet_tag, bullet_sprite);
	bullet->get_component<CRenderer>().color = default_bullet_color_;
	bullet->add_component<CLifeSpan>(bullet_life_span_);

	const float window_height_scale = frame_context.window_size.y / 100.0f;
	bullet->speed = bullet_speed * window_height_scale;
	bullet->dir = bullet_dir;
}

/// Updates the enemy's position by moving it along an oval (ellipse) trajectory.
/// @param frame_context Per-frame context containing the current window size and clock.
void Enemy::update(const FrameContext& frame_context) {
	if (!has_component<CTransform>()) return;
	auto& transform = get_component<CTransform>();

	Vec2 current_trajectory_size{
		trajectory_size.x * (frame_context.window_size / 100.0f).x,
		trajectory_size.y * (frame_context.window_size / 100.0f).y
	};

	transform.pos = oval_trajectory(transform.pos, current_trajectory_size, frame_context.clock, trajectory_speed);
}