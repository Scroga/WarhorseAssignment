#include "entities/enemy_system.h"

EnemySystem::EnemySystem(EntityManager& entity_manager, Vec2 enemies_spawn_pos, Vec2 ufo_spawn_pos, void* enemy_sprite, void* ufo_sprite, void* bullet_sprite)
	: entity_manager_(entity_manager)
	, enemies_spawn_pos_(enemies_spawn_pos), ufo_spawn_pos_(ufo_spawn_pos)
	, enemy_sprite_(enemy_sprite), ufo_sprite_(ufo_sprite), bullet_sprite_(bullet_sprite)
{
	ufo_spawn_timer_.try_trigger(ufo_spawn_cooldown_);
}

/// Creates a grid of enemies with the specified number of rows and columns.
/// The higher enemy is, the faster his bullets are, and the more points he gets.
/// @param columns_count specifies the number of enemies in a single row of the grid.
/// @param rows_count specifies the number of enemies in a single column of the grid.
void EnemySystem::spawn_enemies(size_t columns_count, size_t rows_count) {
	if (columns_count == 0 || rows_count == 0) {
		enemies_.clear();
		return;
	}

	// Recreate the container, one queue per column
	enemies_ = std::vector<EnemyQueue>(columns_count);

	// Compute the top left position so that the whole enemy group is centered at enemies_spawn_pos_
	float row_length = enemy_size_.x * enemies_margin_.x * (columns_count - 1);
	float column_length = enemy_size_.y * enemies_margin_.y * (rows_count - 1);
	Vec2 current_spawn_pos{
		enemies_spawn_pos_.x - row_length * 0.5f,
		enemies_spawn_pos_.y + column_length * 0.5f };


	for (size_t i = 0; i < columns_count; i++) {
		for (size_t j = 0; j < rows_count; j++) {
			// Compute the position of a specific enemy in the grid
			Vec2 pos{
				current_spawn_pos.x + (enemy_size_.x * enemies_margin_.x * i),
				current_spawn_pos.y - (enemy_size_.y * enemies_margin_.y * j)
			};

			// Spawn and setup enemy
			auto enemy = entity_manager_.spawn_actor<Enemy>(
				pos, enemy_size_, enemy_angle_,
				enemy_collision_offset_, enemy_collision_size,
				enemy_tag, enemy_sprite_);

			// Set color, trajectory, bullet speed and points it gets based on its position
			enemy->trajectory_size = enemies_trajectory_size;
			enemy->trajectory_speed = enemies_trajectory_speed;
			enemy->bullet_speed = enemy_bullet_speed_ + enemy_bullet_acceleration_ * j;
			enemy->add_component<CScore>(enemy_score_ + enemy_score_bust_ * j);
			enemy->get_component<CRenderer>().color = offset_color(default_enemy_color_, j + 1);

			// Store the enemy in the corresponding column queue
			enemies_[i].push(enemy);
		}
	}
}

/// The closest enemy in front to the player shoots if the player is within its detection area.
/// @param player shared pointer to the player entity.
void EnemySystem::handle_enemy_shooting(const FrameContext& frame_context, Vec2 player_pos, Vec2 player_size) {
	if (enemy_shoot_timer_.is_running()) return;

	for (auto& enemy_queue : enemies_) {
		auto enemy = get_upper_active_enemy(enemy_queue);
		if (!enemy) continue;

		if (!enemy->has_component<CTransform>()) continue;
		auto& e_transfrom = enemy->get_component<CTransform>();

		// If the player is within the enemy's shooting range, the enemy shoots and triggers the timer
		if (std::abs(player_pos.x - e_transfrom.pos.x) < player_size.x * enemy_shooting_range_multiplier_) {
			enemy->shoot(entity_manager_, frame_context, bullet_sprite_);
			enemy_shoot_timer_.try_trigger(enemy_shoot_cooldown_);
			return;
		}
	}
}

/// Returns first alive enemy in the queue.
/// @param queue of enemies.
/// @return ptr to the uppermost alive enemy.
std::shared_ptr<Enemy> EnemySystem::get_upper_active_enemy(EnemyQueue& queue) {
	while (!queue.empty()) {
		if (auto enemy = queue.front().lock())
			return enemy;
		queue.pop(); // remove expired
	}
	return nullptr;
}

/// Spawns and initialize UFO enemy if the spawn timer is not currently running.
void EnemySystem::handle_spawn_ufo() {
	if (ufo_spawn_timer_.is_running()) return;

	// Spawn and setup UFO
	auto ufo = entity_manager_.spawn_actor<Ufo>(
		ufo_spawn_pos_, ufo_size_, enemy_angle_,
		enemy_collision_offset_, enemy_collision_size,
		enemy_tag, ufo_sprite_);
	ufo->add_component<CLifeSpan>(ufo_life_span_);
	ufo->add_component<CScore>(ufo_score_);

	ufo_spawn_timer_.try_trigger(ufo_spawn_cooldown_);;
}

/// Updates the enemy system for the current frame.
/// Handles enemies shooting and UFO spawn.
/// @param frame_context Per-frame context used for timing and scaling.
/// @param player Player entity used for enemy shooting logic.
void EnemySystem::update(const FrameContext& frame_context, std::shared_ptr<Player> player) {
	if (player->has_component<CTransform>()) {
		auto& p_transform = player->get_component<CTransform>();
		handle_enemy_shooting(frame_context, p_transform.pos, p_transform.size);
	}

	handle_spawn_ufo();
}

/// Clears all containers and release timers.
void EnemySystem::clear() {
	enemies_.clear();
	enemy_shoot_timer_.release();
	ufo_spawn_timer_.release();
}
