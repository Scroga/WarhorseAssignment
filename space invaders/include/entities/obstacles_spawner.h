#pragma once
#include "systems/entity_manager.h"
#include "entities/obstacle.h"
#include "utils/utility.h"

class ObstaclesSpawner {
private:
	const std::vector<Vec2> obstacle_strucute_local_pos_{
		Vec2{0.0f, 0.0f}, Vec2{1.0f, 0.0f}, Vec2{-1.0f, 0.0f}
	};

	const Vec2 obstacle_size_{ 1.5f };
	const float obstacle_angle_ = deg_to_rad(0.0f);
	const Vec2 obstacle_collision_offset_{ 0.0f };
	const float obstacle_collision_size_ = 1.5f;
	const size_t obstacle_health_ = 7;

	EntityManager& entity_manager_;
	std::vector<void*> obstacle_sprites_;
public:
	static constexpr char* obstacle_tag = "obstacle";
	ObstaclesSpawner() = delete;
	ObstaclesSpawner(EntityManager& entity_manager, std::vector<void*> obstacle_sprites);

	std::shared_ptr<Obstacle> spawn_one_obstacle(Vec2 pos);
	void spawn_obstacle_structure(Vec2 pos);
	void clear();
};