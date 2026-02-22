#include "entities/player.h"

#include "systems/asset_manager.h"
#include "systems/entity_manager.h"
#include "utils/collision_detector.h"

Player::Player(const size_t id, const std::string& tag) : Entity(id, tag) {}

/// Spawns player's bullet and initializes its rendering, lifetime, and movement.
/// @param entity_manager Entity manager used to spawn the bullet entity.
/// @param frame_context Per-frame context (used for current window size).
/// @param bullet_sprite Sprite handle used to render the bullet.
void Player::shoot(EntityManager& entity_manager, const FrameContext& frame_context, void* bullet_sprite) {
	if (!shoot_timer_.try_trigger(enemy_shoot_cooldown_)) return;

	if (!has_component<CTransform>()) return;
	auto& transform = get_component<CTransform>();

	// Spawn and setup bullet
	auto bullet = entity_manager.spawn_actor<Bullet>(
		px_to_pos(transform.pos, frame_context.window_size) + bullet_offset_,
		bullet_size_, bullet_angle_,
		bullet_collision_offset_, bullet_collision_size_,
		bullet_tag, bullet_sprite);
	bullet->add_component<CLifeSpan>(bullet_life_span_);

	const float window_height_scale = frame_context.window_size.y / 100.0f;
	bullet->speed = bullet_speed_ * window_height_scale;
	bullet->dir = bullet_dir;
}

/// Moves the player horizontally in the specified direction.
/// Checks for collisions with the window's horizontal boundaries.
/// @param dir Horizontal movement direcction (LEFT or RIGHT).
/// @param frame_context Per-frame context containing the current window size and clock.
void Player::move(MovementDir dir, const FrameContext& frame_context) {
	if (!has_component<CTransform>() || !has_component<CCollision>()) return;
	auto& transform = get_component<CTransform>();
	auto& collision = get_component<CCollision>();

	int d = (dir == MovementDir::RIGHT) ? 1 : -1;
	const float window_width_scale = frame_context.window_size.x / 100.0f;
	// scale speed based on window width and delta time for resolution/time independence
	const float current_speed = movement_speed * window_width_scale * frame_context.clock.get_delta_time();
	float new_x_pos = transform.pos.x + d * current_speed;

	if (!CollisionDetector::hits_window_x(frame_context.window_size.x, new_x_pos, collision.radius)) {
		transform.pos.x = new_x_pos;
	}
}

/// Changes player's color for a short period of time and decreases its health.
void Player::on_damage() {
	if (!has_component<CRenderer>() || !has_component<CHealth>()) return;
	auto& renderer = get_component<CRenderer>();
	auto& health = get_component<CHealth>();

	renderer.color = DAMAGE_COLOR;
	color_chage_timer_.release();
	color_chage_timer_.try_trigger(damage_color_change_);
	
	health.current_hp--;
}

/// Handles player's vertical bobbing and restores the normal color after taking damage.
/// @param frame_context Per-frame context containing the current clock used for player's bobbing.
void Player::update(const FrameContext& frame_context) {
	if (has_component<CTransform>()) {
		auto& transform = get_component<CTransform>();
		transform.pos.y = bobbing(frame_context.clock, transform.pos.y, bobbing_freq, bobbing_scale);
	}
 
	if (!has_component<CTransform>()) return;
	auto& renderer = get_component<CRenderer>();
	if (!color_chage_timer_.is_running() && renderer.color != NORMAL_COLOR) {
		renderer.color = NORMAL_COLOR;
	}
}