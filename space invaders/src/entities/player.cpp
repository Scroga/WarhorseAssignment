#include "entities/player.h"

#include "systems/asset_manager.h"
#include "systems/entity_manager.h"
#include "utils/collision_detector.h"

Player::Player(const size_t id, const std::string& tag) : Entity(id, tag) {}

void Player::shoot(EntityManager& entity_manager, void* bullet_sprite) {
	if (!shoot_timer_.try_trigger(enemy_shoot_cooldown_)) return;

	if (!has_component<CTransform>()) return;
	auto& transform = get_component<CTransform>();

	// Spawn and setup bullet
	auto bullet = entity_manager.spawn_actor<Bullet>(
		transform.pos + bullet_offset_,
		bullet_size_, bullet_angle_,
		bullet_collision_offset_, bullet_collision_size_,
		bullet_tag, bullet_sprite);
	bullet->speed = bullet_speed_;
	bullet->dir = bullet_dir;
}

// Moves the player in the specified horizontal direction.
// checks for collisions with the window's horizontal boundaries
void Player::move(MovementDir dir, const GameClock& clock, Vec2 widnow_size) {
	if (!has_component<CTransform>() || !has_component<CCollision>()) return;
	auto& transform = get_component<CTransform>();
	auto& collision = get_component<CCollision>();

	int d = (dir == MovementDir::RIGHT) ? 1 : -1;
	float new_x_pos = transform.pos.x + (movement_speed * d * clock.get_delta_time());

	if (!CollisionDetector::hits_window_x(widnow_size.x, new_x_pos, collision.radius)) {
		transform.pos.x = new_x_pos;
	}
}

// Changes the player's color for a short period of time
// and decreases the player's health
void Player::on_damage() {
	if (!has_component<CRenderer>() || !has_component<CHealth>()) return;
	auto& renderer = get_component<CRenderer>();
	auto& health = get_component<CHealth>();

	renderer.color = DAMAGE_COLOR;
	color_chage_timer_.release();
	color_chage_timer_.try_trigger(damage_color_change_);
	
	health.current_hp--;
}

// Handles the player's vertical bobbing and restores the normal color after taking damage
void Player::update(const GameClock& clock) {
	if (has_component<CTransform>()) {
		auto& transform = get_component<CTransform>();
		transform.pos.y = bobbing(clock, transform.pos.y, bobbing_freq, bobbing_scale);
	}
 
	if (!has_component<CTransform>()) return;
	auto& renderer = get_component<CRenderer>();
	if (!color_chage_timer_.is_running() && renderer.color != NORMAL_COLOR) {
		renderer.color = NORMAL_COLOR;
	}
}