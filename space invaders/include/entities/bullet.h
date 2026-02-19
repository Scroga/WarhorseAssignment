#pragma once
#include "entities/entity.h"
#include "utils/collision_detector.h"

enum class BulletDir { UP, DOWN };

class Bullet : public Entity {
protected:
	friend class EntityManager;
	Bullet(const size_t id, const std::string& tag) : Entity(id, tag) {};
public:
	float speed = 500.0f;
	BulletDir dir = BulletDir::UP;

	/////// TODO: ADD BULLET LIFE SPAN !!!!!!!!!!!

	/// Updates the bullet's movement and destroys it when it collides with the window's vertical borders
	/// @param clock Allows updating movement correctly using delta time
	void update(const GameClock& clock) override {
		if (!has_component<CTransform>() || !has_component<CCollision>()) return;

		auto& transform = get_component<CTransform>();
		auto& collision = get_component<CCollision>();

		int d = (dir == BulletDir::UP) ? -1 : 1;
		transform.pos.y += (speed * d * clock.get_delta_time());
	}
};