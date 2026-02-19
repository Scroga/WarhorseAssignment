#pragma once

#include "entities/ufo.h"
#include "entities/enemy.h"
#include "entities/player.h"
#include "utils/game_clock.h"
#include "utils/vec2.h"
#include "utils/utility.h"
#include "systems/entity_manager.h"

#include <queue>
#include <vector>

class EnemySystem {
private:
	using EnemyQueue = std::queue<std::weak_ptr<Enemy>>;

	const DWORD default_enemy_color_ = 0xffffffff;

	const float enemy_shooting_range_multiplier_ = 1.5f;
	const float enemy_bullet_speed_ = 150.0f;
	const float enemy_bullet_acceleration_ = 15.0f;
	const milliseconds enemy_shoot_cooldown_{ 800 };

	const int enemy_score_ = 10;
	const int enemy_score_bust_ = 2;

	const Vec2 enemies_spawn_pos_;
	const Vec2 enemies_margin_{ 2.7f, 1.4f };

	const Vec2 enemy_size_{ 30.0f, 30.0f };
	const float enemy_angle_ = deg_to_rad(0.0f);
	const Vec2 enemy_collision_offset_{ 0.0f, 0.0f };
	const float enemy_collision_size = 30.0f;

	const Vec2 ufo_spawn_pos_;
	const Vec2 ufo_size_{ 40.0f };
	const milliseconds ufo_spawn_cooldown_{ 25000 };
	const int ufo_score_ = 100;
	const int ufo_life_span_ = 2200;

	std::vector<EnemyQueue> enemies_;
	GameTimer enemy_shoot_timer_;
	GameTimer ufo_spawn_timer_;

	EntityManager& entity_manager_;
	void* enemy_sprite_;
	void* ufo_sprite_;
	void* bullet_sprite_;
	
	std::shared_ptr<Enemy> get_upper_active_enemy(EnemyQueue& queue);
	void handle_enemy_shooting(std::shared_ptr<Player> player);
	void handle_spawn_ufo();
public:
	static constexpr const char* enemy_tag = "enemy";

	EnemySystem(EntityManager& entity_manager, Vec2 enemies_spawn_pos, Vec2 ufo_spawn_pos, 
		void* enemy_sprite, void* ufo_sprite, void* bullet_sprite);

	void update(std::shared_ptr<Player> player);
	void spawn_enemies(size_t columns_count, size_t rows_count);
	void clear();
};