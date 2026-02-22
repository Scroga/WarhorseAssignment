#include "utils/game_hud.h"

/// Adds a text element to the HUD under the given tag.
/// @param tag Unique identifier used to store and later access the text.
/// @param text Text element to store in the HUD.
void GameHud::add_text(const std::string& tag, const GameText& text) {
    auto it = texts_.find(tag);
    if (it == texts_.end())
        texts_.emplace(tag, text);
}

/// Returns a pointer to the HUD text stored under the given tag.
/// @param tag Unique identifier of the text element to retrieve.
/// @return Pointer to the stored `GameText`, or nullptr if the tag does not exist.
GameText* GameHud::get_text(const std::string& tag) {
    auto it = texts_.find(tag);
    if (it == texts_.end()) return nullptr;
    return &it->second;
}

/// Adds an entity element to the HUD under the given tag.
/// @param tag Unique identifier used to store and later access the entity.
/// @param e Entity to store in the HUD.
void GameHud::add_entity(const std::string& tag, std::shared_ptr<Entity> e) {
    auto it = entities_.find(tag);
    if (it == entities_.end())
        entities_.emplace(tag, std::move(e));
}

/// Returns a pointer to the HUD text stored under the given tag.
/// @param tag Unique identifier of the text element to retrieve.
/// @return Pointer to the stored `GameText`, or nullptr if the tag does not exist.
std::shared_ptr<Entity> GameHud::get_entity(const std::string& tag){
    auto it = entities_.find(tag);
    if (it == entities_.end()) return nullptr;
    return it->second.lock(); // may return nullptr if entity is gone
}

/// Draws all added texts.
void GameHud::draw_texts(const SpriteRenderer& renderer) {
    for (auto& t : texts_) {
        t.second.draw(renderer);
    }
}

/// Remove all texts and entities.
void GameHud::clear() {
    texts_.clear();
    entities_.clear();
}