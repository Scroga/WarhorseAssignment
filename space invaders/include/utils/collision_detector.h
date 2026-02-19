#pragma once

#include "game_application.h"
#include "entities/entity.h"
#include "utils/vec2.h"

#include <memory>

class CollisionDetector {
public:
	/// Checks whether two entities are in collision.
	/// @param a first entity.
	/// @param b second entity.
	/// @return true if two entites are in collision.
	static bool has_collision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
		if (!a->has_component<CTransform>() || !b->has_component<CTransform>() ||
			!a->has_component<CCollision>() || !b->has_component<CCollision>())
			return false;

		auto& a_transform = a->get_component<CTransform>();
		auto& a_collision = a->get_component<CCollision>();

		auto& b_transform = b->get_component<CTransform>();
		auto& b_collision = b->get_component<CCollision>();

		// Calculate the global position of the collision circles.
		Vec2 a_pos = a_transform.pos + a_collision.local_pos;
		Vec2 b_pos = b_transform.pos + b_collision.local_pos;

		// Calculate the distance between the collision circles.
		float dist = a_pos.distance(b_pos);

		// If the distance is less then the sum of the radii, entities are in collision. 
		return dist < (a_collision.radius + b_collision.radius);
	}
	
	/// Checks collision against the window borders on the X axis.
	/// @param pos_x entity's X position.
	/// @param col_radius entity's collision radius.
	/// @return true if colliding with the window on X, otherwise false.
	static bool hits_window_x(float window_width, float pos_x, float col_radius) {
		return
			(pos_x - col_radius < 0.0f) ||
			(pos_x + col_radius > window_width);
	}

	/// Checks collision against the window borders on the Y axis.
	/// @param pos_y entity's Y position.
	/// @param col_radius entity's collision radius.
	/// @return true if colliding with the window on Y, otherwise false.
	static bool hits_window_y(float window_height, float pos_y, float col_radius) {
		return
			(pos_y - col_radius < 0.0f) ||
			(pos_y + col_radius > window_height);
	}
};