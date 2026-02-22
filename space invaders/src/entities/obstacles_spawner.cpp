#include "entities/obstacles_spawner.h"
#include "systems/entity_manager.h"
#include "systems/asset_manager.h"


ObstaclesSpawner::ObstaclesSpawner(EntityManager& entity_manager, std::vector<void*> obstacle_sprites)
	: entity_manager_(entity_manager), obstacle_sprites_(std::move(obstacle_sprites)) {
}

/// Spawns a single obstacle entity at the given position and initializes its state.
/// @param pos Position where the obstacle should be spawned.
/// @return Shared pointer to the newly spawned obstacle entity.
std::shared_ptr<Obstacle> ObstaclesSpawner::spawn_one_obstacle(Vec2 pos) {
	auto obstacle = entity_manager_.spawn_actor<Obstacle>(
		pos, obstacle_size_, obstacle_angle_, 
		obstacle_collision_offset_, obstacle_collision_size_,
		obstacle_tag, obstacle_sprites_[0]);

	obstacle->set_sprites(obstacle_sprites_);
	obstacle->add_component<CHealth>(obstacle_health_);
	return obstacle;
}

/// Spawns a structure made of multiple obstacles arranged in a predefined pattern.
/// @param pos Position used as the origin/anchor for the whole obstacle structure.
void ObstaclesSpawner::spawn_obstacle_structure(Vec2 pos) {
	for (Vec2 local_pos : obstacle_strucute_local_pos_) {
		Vec2 global_pos{
			(local_pos.x * obstacle_size_.x * 2.0f) + pos.x,
			(local_pos.y * obstacle_size_.y * 2.0f) + pos.y };
		spawn_one_obstacle(global_pos);
	}
}

void ObstaclesSpawner::clear() {
	obstacle_sprites_.clear();
}