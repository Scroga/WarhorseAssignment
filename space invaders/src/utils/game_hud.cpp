#include "utils/game_hud.h"

void GameHud::add_text(const std::string& tag, const GameText& text) {
    auto it = texts_.find(tag);
    if (it == texts_.end())
        texts_.emplace(tag, text);
}

GameText* GameHud::get_text(const std::string& tag) {
    auto it = texts_.find(tag);
    if (it == texts_.end()) return nullptr;
    return &it->second;
}

void GameHud::add_entity(const std::string& tag, std::shared_ptr<Entity> e) {
    entities_[tag] = std::move(e);
}

std::shared_ptr<Entity> GameHud::get_entity(const std::string& tag) {
    auto it = entities_.find(tag);
    if (it == entities_.end()) return nullptr;
    return it->second.lock(); // may return nullptr if entity is gone
}

void GameHud::draw(const SpriteRenderer& renderer) {
    for (auto& t : texts_) {
        t.second.draw(renderer);
    }
}

void GameHud::clear() {
    texts_.clear();
    entities_.clear();
}