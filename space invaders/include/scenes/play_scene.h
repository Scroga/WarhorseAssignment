#pragma once

#include "scene.h"

#include "entities/enemy_system.h"
#include "entities/obstacles_spawner.h"

#include "utils/game_hud.h"
#include "utils/health_bar.h"
#include "utils/game_text.h"
#include "utils/game_clock.h"
#include "utils/utility.h"
#include "utils/vec2.h"

#include "entities/player.h"

class PlayScene : public Scene {
private:
	// player settings
	const Vec2 player_spawn_pos_{ 50, 92 };
	const Vec2 player_size_{ 2.5f };
	const float player_angle_ = deg_to_rad(0.0f);
	const Vec2 player_collision_offset_{ 0.0f, 0.0f };
	const float player_collision_size_ = 2.0f;
	const float player_speed_ = 50.0f;
	const size_t max_player_health_ = 3;

	std::shared_ptr<Player> player_;

	// enemies settings
	const Vec2 enemies_spawn_pos_{ 50, 45 };
	const Vec2 ufo_spawn_pos_{ 110, 13 };
	const Vec2i enemies_grid_size_{ 11, 6 };

	EnemySystem enemy_system_{
		services_.entity_manager, enemies_spawn_pos_, ufo_spawn_pos_,
		services_.asset_manager.get_enemy_sprite(),
		services_.asset_manager.get_ufo_sprite(),
		services_.asset_manager.get_bullet_sprite(),
	};

	// obstacles settings
	const std::vector<Vec2> obstacles_spawn_pos_{ Vec2{50, 75}, Vec2{25, 80}, Vec2{75, 80} };
	
	ObstaclesSpawner obstacles_spawner_{
		services_.entity_manager,
		services_.asset_manager.get_obstacle_sprites()
	};

	// health bar settings
	const std::string score_tag_ = "score";
	const Vec2 health_bar_pos_{ 20, 5 };
	const Vec2 health_bar_szie_{ 1.5f };
	
	HealthBar health_bar_{
		services_.entity_manager,
		health_bar_pos_, health_bar_szie_,
		services_.asset_manager.get_player_sprite(), max_player_health_ };

	bool process_enemy_hit(std::shared_ptr<Entity> bullet);
	bool process_player_hit(std::shared_ptr<Entity> bullet);
	bool process_obstacle_hit(std::shared_ptr<Entity> bullet);

public:
	static constexpr const char* player_tag = "player";
	bool render_collision = true;

	PlayScene(SceneType type, GameServices& services);

	void setup_hud() override;
	void draw_entities() override;
	void handle_input() override;
	void update() override;
	void clear() override;
	int get_score() const;
};