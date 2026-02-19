#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "entities/entity.h"
#include "utils/game_text.h"

using DWORD = unsigned long;

class GameHud {
private:
	std::map<std::string, GameText> texts_;
	std::map<std::string, std::weak_ptr<Entity>> entities_;

public:
	GameHud() = default;
	void add_text(const std::string& tag, const GameText& text);
	GameText* get_text(const std::string& tag);
	
	void add_entity(const std::string& tag, std::shared_ptr<Entity> text);
	std::shared_ptr<Entity> get_entity(const std::string& tag);

	void draw(const SpriteRenderer& renderer);
	void clear();
};