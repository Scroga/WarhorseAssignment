#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <type_traits>
#include <map>

#include "entities/entity.h"

using EntityVector = std::vector<std::shared_ptr<Entity>>;

class Enemy;

class EntityManager {
private:
	EntityVector					    entities_;			// all entities
	EntityVector						entities_to_add_;   // entities to add in the next frame
	std::map<std::string, EntityVector> entity_map_;		// map for entity tag to vectors
	size_t								total_entity_count = 0;

	const Vec2 window_size_;

	void remove_dead_entities(EntityVector& vector);
public:
	EntityManager(Vec2 window_size) : window_size_(window_size) {}

	void update();

	template<typename TEntity, typename = std::enable_if_t<std::is_base_of<Entity, TEntity>::value>>
	std::shared_ptr<TEntity> add_entity(const std::string& tag) {
		// it should be with 'new' because TEntity has private ctr
		auto entity_ptr = std::shared_ptr<TEntity>(new TEntity(total_entity_count++, tag));

		entities_to_add_.push_back(entity_ptr);
		return entity_ptr;
	}

	const EntityVector& get_entities() const { return entities_; }
	const EntityVector& get_entities(const std::string& tag) const;
	std::shared_ptr<Entity> get_entity(const std::string& tag, size_t id) const;

	/// Spawns an entity that will have CTransform, CCollision and CRenderer componentes.
	template<typename TEntity, typename = std::enable_if_t<std::is_base_of<Entity, TEntity>::value>>
	std::shared_ptr<TEntity> spawn_actor(
		Vec2 spawn_pos, Vec2 size, float angle_rad,
		Vec2 collision_offset, float collision_size, 
		const std::string& tag, void* sprite) {

		auto entity = add_entity<TEntity>(tag);

		entity->add_component<CTransform>(
			pos_to_px(spawn_pos, window_size_),
			size_to_px(size, window_size_), 
			angle_rad);
		entity->add_component<CCollision>(
			pos_to_px(collision_offset, window_size_),
			size_to_px(collision_size, window_size_));

		assert(sprite && "Entity sprite is null: did you preload assets?");
		entity->add_component<CRenderer>(sprite);

		return entity;
	}

	/// Spawns an entity that will have CTransform and CRenderer componentes.
	template<typename TEntity, typename = std::enable_if_t<std::is_base_of<Entity, TEntity>::value>>
	std::shared_ptr<TEntity> spawn_sprite(
		Vec2 spawn_pos, Vec2 size, float angle_rad, const std::string& tag, void* sprite) {
		auto entity = add_entity<TEntity>(tag);

		entity->add_component<CTransform>(
			pos_to_px(spawn_pos, window_size_),
			size_to_px(size, window_size_),
			angle_rad);

		assert(sprite && "Entity sprite is null: did you preload assets and is the path correct?");
		entity->add_component<CRenderer>(sprite);

		return entity;
	}
	
	void clear();
};