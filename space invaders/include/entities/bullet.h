#pragma once
#include "entities/entity.h"
#include "utils/collision_detector.h"

enum class BulletDir { UP, DOWN };

class Bullet : public Entity {
protected:
	friend class EntityManager;
	Bullet(const size_t id, const std::string& tag) : Entity(id, tag) {};
public:
	float speed = 50.0f;
	BulletDir dir = BulletDir::UP;

	/// Updates the bullet's movement.
	/// @param frame_context Allows updating movement correctly using delta time.
	void update(const FrameContext& frame_context) override {
		if (!has_component<CTransform>() || !has_component<CCollision>()) return;

		auto& transform = get_component<CTransform>();
		auto& collision = get_component<CCollision>();

		int d = (dir == BulletDir::UP) ? -1 : 1;
		transform.pos.y += (speed * d * frame_context.clock.get_delta_time());
	}
};