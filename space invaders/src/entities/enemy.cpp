#include "entities/enemy.h"
#include "systems/asset_manager.h"
#include "systems/entity_manager.h"

Enemy::Enemy(const size_t id, const std::string& tag) : Entity(id, tag) {}

void Enemy::shoot(EntityManager& entity_manager, void* bullet_sprite) {
	if (!has_component<CTransform>()) return;
	auto& transform = get_component<CTransform>();

	// Spawn and setup bullet
	auto bullet = entity_manager.spawn_actor<Bullet>(
		transform.pos + bullet_offset_,
		bullet_size_, bullet_angle_,
		bullet_collision_offset_, bullet_collision_size_,
		bullet_tag, bullet_sprite);
	bullet->speed = bullet_speed;
	bullet->dir = bullet_dir;
	bullet->add_component<CLifeSpan>(bullet_life_span_);
	bullet->get_component<CRenderer>().color = default_bullet_color_;
}

void Enemy::update(const GameClock& clock) {
	if (!has_component<CTransform>()) return;
	auto& transform = get_component<CTransform>();

	transform.pos.x = bobbing(clock, transform.pos.x, bobbing_freq, bobbing_scale);
}