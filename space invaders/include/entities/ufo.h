#pragma once
#include "entities/entity.h"
#include "utils/utility.h"

class Ufo : public Entity {
protected:
	friend class EntityManager;
	Ufo(const size_t id, const std::string& tag) : Entity(id, tag) {}
public:
	MovementDir dir = MovementDir::LEFT;

	float speed = 100.0f;
	float bobbing_freq = 0.01f;
	float bobbing_scale = -35.0f;

	/// Updates the UFO's movement along the x-axis and applies vertical bobbing
	/// @param clock Allows updating movement correctly using delta time
	void update(const GameClock& clock) override {
		if (!has_component<CTransform>()) return;
		auto& transform = get_component<CTransform>();

		int d = (dir == MovementDir::RIGHT) ? 1 : -1;

		transform.pos.x = transform.pos.x + (speed * d * clock.get_delta_time());
		transform.pos.y = bobbing(clock, transform.pos.y, bobbing_freq, bobbing_scale);
	}
};
