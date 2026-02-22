#include "systems/entity_manager.h"

#include "entities/enemy.h"

#include <algorithm>

/// Updates the entity manager for the current frame.
void EntityManager::update() {
	// Removes dead entities from the main vector
	remove_dead_entities(entities_);

	// Removes dead entities from the tag-to-entities map
	for (auto& kv : entity_map_)
		remove_dead_entities(kv.second);

	// Add new entities which were created in the previous frame
	for (auto entity : entities_to_add_) {
		entities_.push_back(entity);
		entity_map_[entity->tag()].push_back(entity);
	}
	entities_to_add_.clear();
}

/// Removes entities that are no longer alive from the given container.
void EntityManager::remove_dead_entities(EntityVector& vector) {
	vector.erase(std::remove_if(vector.begin(), vector.end(),
		[](const auto& e) {return !e->is_alive(); }), vector.end());
}

/// Returns a read-only list of entities that have the given tag.
const EntityVector& EntityManager::get_entities(const std::string& tag) const {
	static const EntityVector empty;
	auto it = entity_map_.find(tag);
	return (it == entity_map_.end()) ? empty : it->second;
}

/// Searches for an entity with the specified tag and id
/// @param tag Tag used when the entity was created
/// @param id Unique entity id assigned at creation time by entity manager
/// @return Shared pointer to the entity
std::shared_ptr<Entity> EntityManager::get_entity(const std::string& tag, size_t id) const {
	const auto& entities = get_entities(tag);
	auto it = std::find_if(entities.begin(), entities.end(),
		[&](const std::shared_ptr<Entity>& e) {
			return e && e->id() == id;
		});
	return (it == entities.end()) ? nullptr : *it;
}

/// Removes all entities in containers, sets total entity count to zero.
void EntityManager::clear() {
	entities_.clear();
	entities_to_add_.clear();
	entity_map_.clear();
	total_entity_count = 0;
}