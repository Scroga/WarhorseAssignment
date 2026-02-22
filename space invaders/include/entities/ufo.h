#pragma once
#include "entities/entity.h"
#include "utils/utility.h"

class Ufo : public Entity {
protected:
	friend class EntityManager;
	Ufo(const size_t id, const std::string& tag) : Entity(id, tag) {}
public:
	MovementDir dir = MovementDir::LEFT;

	float speed = 10.0f;
	float bobbing_freq = 0.05f;
	float bobbing_scale = -35.0f;

	/// Updates the UFO's movement along the x-axis and applies vertical bobbing.
	/// @param frame_context Allows updating movement correctly using delta time and screeb size.
	void update(const FrameContext& frame_context) override {
		if (!has_component<CTransform>()) return;
		auto& transform = get_component<CTransform>();

		int d = (dir == MovementDir::RIGHT) ? 1 : -1;

		float current_speed = speed * frame_context.clock.get_delta_time() * frame_context.window_size.x / 100.0f;
		transform.pos.x = transform.pos.x + current_speed * d;
		transform.pos.y = bobbing(frame_context.clock, transform.pos.y, bobbing_freq, bobbing_scale);
	}
};
