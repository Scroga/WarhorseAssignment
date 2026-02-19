#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "systems/entity_manager.h"
#include "utils/game_text.h"
#include "utils/vec2.h"
#include "utils/utility.h"

class HealthBar {
private:
	static constexpr DWORD DEFAULT_COLOR = 0xffffffff;
	static constexpr DWORD DAMAGE_COLOR = 0x00fffff;

	EntityManager& entity_manager_;

	Vec2 pos_;
	Vec2 size_;
	size_t points_;
	void* sprite_;
	size_t points_ptr_;

	std::vector<std::weak_ptr<Entity>> sprites_;
public:
	float margin = 2.4f;
	HealthBar() = delete;
	HealthBar(EntityManager& entity_manager, Vec2 pos, Vec2 s, void* spr, size_t p);

	void reset();
	void increase();
	void decrease();
	void clear();
};