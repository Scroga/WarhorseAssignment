#pragma once

#include "scene.h"

#include "entities/enemy_system.h"
#include "entities/obstacles_spawner.h"

#include "utils/game_hud.h"
#include "utils/health_bar.h"
#include "utils/game_text.h"
#include "utils/game_clock.h"
#include "utils/vec2.h"

#include "entities/player.h"

class PlayScene : public Scene {
private:
	const std::string SCORE_TAG = "score";
	const Vec2 health_bar_pos_{ services_.window_size.x / 8.0f,  services_.window_size.y / 9.0f };
	const Vec2 health_bar_szie_{ 20.0f };

	const std::vector<Vec2> obstacles_spawn_pos_{
		Vec2{ services_.window_size.x / 2.0f, services_.window_size.y - services_.window_size.y / 4.5f },
		Vec2{ services_.window_size.x / 5.0f, services_.window_size.y - services_.window_size.y / 5.0f },
		Vec2{ services_.window_size.x - services_.window_size.x / 5.0f, services_.window_size.y - services_.window_size.y / 5.0f }
	};

	const Vec2 ufo_spawn_pos_{ services_.window_size.x + services_.window_size.x / 8.0f, services_.window_size.y / 8.0f };
	const Vec2 enemies_spawn_pos_{ services_.window_size.x / 2.0f, services_.window_size.y / 2.5f };
	const Vec2i enemies_grid_size_{ 11, 6 };

	const Vec2 player_spawn_pos_{ services_.window_size.x / 2.0f, services_.window_size.y - services_.window_size.y / 12.0f };

	const Vec2 player_size_{ 30.0f, 30.0f };
	const float player_angle_ = deg_to_rad(0.0f);
	const Vec2 player_collision_offset_{ 0.0f, 0.0f };
	const float player_collision_size_ = 30.0f;
	const float player_speed_ = 600.0f;
	const size_t max_player_health_ = 3;

	std::shared_ptr<Player> player_;

	EnemySystem enemy_system_{ 
		services_.entity_manager, enemies_spawn_pos_, ufo_spawn_pos_,
		services_.asset_manager.get_enemy_sprite(),
		services_.asset_manager.get_ufo_sprite(),
		services_.asset_manager.get_bullet_sprite(),
	};

	ObstaclesSpawner obstacles_spawner_{
		services_.entity_manager,
		services_.asset_manager.get_obstacle_sprites()
	};

	HealthBar health_bar_{
		services_.entity_manager,
		health_bar_pos_, health_bar_szie_, 
		services_.asset_manager.get_player_sprite(), max_player_health_ };

	bool process_enemy_hit(std::shared_ptr<Entity> bullet);
	bool process_player_hit(std::shared_ptr<Entity> bullet);
	bool process_obstacle_hit(std::shared_ptr<Entity> bullet);

public:
	static constexpr char* player_tag = "player";
	bool render_collision = false;

	PlayScene(SceneType type, GameServices& services);

	void setup_hud() override;
	void draw_entities() override;
	void handle_input(const GameClock& clock) override;
	void update(const GameClock& clock) override;
	void clear() override;
	int get_score() const;
};