#include "scenes/play_scene.h"

#include <sstream>

PlayScene::PlayScene(SceneType type, GameServices& services) : Scene(type, services) {
	// Spawn obstacles
	for (auto& obstacle_pos : obstacles_spawn_pos_)
		obstacles_spawner_.spawn_obstacle_structure(obstacle_pos);

	// Spawn and setup player
	player_ = services_.entity_manager.spawn_actor<Player>(
		player_spawn_pos_, player_size_, player_angle_,
		player_collision_offset_, player_collision_size_,
		player_tag, services_.asset_manager.get_player_sprite());

	player_->add_component<CScore>();
	player_->add_component<CHealth>(max_player_health_);
	player_->movement_speed = player_speed_;

	setup_hud();
}

/// Creates HUD text elements and stores the changeable ones in game_hud_ under specific tags,
/// so they can be accessed and updated later. Static text that will not change does not use constant tags
void PlayScene::setup_hud() {
	Vec2 text_size{ 15.0f };

	Vec2 window_size = services_.window_size;

	Vec2 score_text_pos{ window_size.x - window_size.x / 10.0f, window_size.y / 20.0f };
	auto score_text = GameText(services_.asset_manager, CTransform(score_text_pos, text_size), "score");

	Vec2 score_point_pos{ window_size.x - window_size.x / 10.0f, window_size.y / 10.0f };
	auto score_points = GameText(services_.asset_manager, CTransform(score_point_pos, text_size), 0);

	Vec2 health_text_pos{ window_size.x / 10.0f, window_size.y / 20.0f };
	auto health_text = GameText(services_.asset_manager, CTransform(health_text_pos, text_size), "health");

	// we will not update these texts, so we don't need to remember their tags
	game_hud_.add_text("score_text", score_text);
	game_hud_.add_text("health_text", health_text);

	game_hud_.add_text(SCORE_TAG, score_points);
}

/// Iterates through each entity in the entity manager and draws its sprite
/// Draws the entity's collision sphere if render_collision is set to true
void PlayScene::draw_entities() {
	for (auto& entity : services_.entity_manager.get_entities()) {
		if (!entity->has_component<CTransform>()) continue;
		auto& transform = entity->get_component<CTransform>();

		if (entity->has_component<CRenderer>()) {
			auto& renderer = entity->get_component<CRenderer>();

			services_.sprite_renderer.draw_sprite(
				renderer.sprite,
				transform.pos.x, transform.pos.y,
				transform.size.x, transform.size.y,
				transform.angle, renderer.color);
		}

		if (render_collision && entity->has_component<CCollision>()) {
			auto& collision = entity->get_component<CCollision>();

			float pos_x = transform.pos.x + collision.local_pos.x;
			float pos_y = transform.pos.y + collision.local_pos.y;
			services_.sprite_renderer.draw_circle(pos_x, pos_y, collision.radius);
		}
	}

	game_hud_.draw(services_.sprite_renderer);
}

void PlayScene::handle_input(const GameClock& clock) {
	auto& p_transform = player_->get_component<CTransform>();

	if (services_.input_handler.is_key_down(VK_SPACE)) {
		player_->shoot(services_.entity_manager, services_.asset_manager.get_bullet_sprite());
	}
	if (services_.input_handler.is_key_down(VK_RIGHT)) {
		player_->move(MovementDir::RIGHT, clock, services_.window_size);
	}
	else if (services_.input_handler.is_key_down(VK_LEFT)) {
		player_->move(MovementDir::LEFT, clock, services_.window_size);
	}
}

void PlayScene::update(const GameClock& clock) {
	// Check player's bullets collision with enemies and obstacles
	for (auto& p_bullet : services_.entity_manager.get_entities(Player::bullet_tag)) {
		if (process_enemy_hit(p_bullet)) continue;
		else if (process_obstacle_hit(p_bullet)) continue;
	}
	// Check enemies's bullet collision with player and obstacles
	for (auto& e_bullet : services_.entity_manager.get_entities(Enemy::bullet_tag)) {
		if (process_player_hit(e_bullet)) continue;
		else if (process_obstacle_hit(e_bullet)) continue;
	}

	// Update entities components
	for (auto& entity : services_.entity_manager.get_entities()) {
		if (!entity->is_alive()) continue;
		entity->update(clock);

		// Update life span
		if (entity->has_component<CLifeSpan>()) {
			auto& duration = entity->get_component<CLifeSpan>();
			if (duration.duration < 1) entity->destroy();
			else duration.duration--;
		}

		// Update health
		if (entity->has_component<CHealth>()) {
			auto& health = entity->get_component<CHealth>();
			if (health.current_hp < 1) entity->destroy();
		}
	}

	// Spawn a new enemy grid if all enemies were killed
	if (services_.entity_manager.get_entities(EnemySystem::enemy_tag).empty()) {
		enemy_system_.spawn_enemies(enemies_grid_size_.x, enemies_grid_size_.y);
	}
	enemy_system_.update(player_);

	if (!player_->is_alive()) { 
		end_scene(); 
	}
}

/// Takes a bullet (usually the player's), iterates through each enemy, and checks for a collision with the bullet
/// If a collision occurs, destroys the enemy and the bullet, and updates the player's score
/// @return true if a collision occurred, otherwise false
bool PlayScene::process_enemy_hit(std::shared_ptr<Entity> bullet) {
	for (auto& enemy : services_.entity_manager.get_entities(EnemySystem::enemy_tag)) {
		// Destoy enemy if it collides with the player's bullet
		if (CollisionDetector::has_collision(enemy, bullet)) {
			if (enemy->has_component<CScore>()) {
				auto& p_score = player_->get_component<CScore>();
				auto& e_score = enemy->get_component<CScore>();

				p_score.amount += e_score.amount;
				game_hud_.get_text(SCORE_TAG)->set_text(p_score.amount);
			}

			enemy->destroy();
			bullet->destroy();
			return true;
		}
	}
	return false;
}

/// Takes a bullet (usually the entity's), checks for collisions between the bullet and the player
/// If a collision occurs, destroys the bullet, damages player and updates health bar
/// @return true if a collision occurred, otherwise false
bool PlayScene::process_player_hit(std::shared_ptr<Entity> bullet) {
	if (CollisionDetector::has_collision(bullet, player_)) {
		bullet->destroy();
		player_->on_damage();
		health_bar_.decrease();
		return true;
	}
	return false;
}

/// Takes a bullet, iterates through the obstacles, and checks for collisions
/// If a collision occurs, destroys the bullet and calls the obstacle's damage method
/// @return true if a collision occurred, otherwise false
bool PlayScene::process_obstacle_hit(std::shared_ptr<Entity> bullet) {
	for (auto& obstacle : services_.entity_manager.get_entities(ObstaclesSpawner::obstacle_tag)) {
		if (!obstacle->is_alive()) continue;
		if (CollisionDetector::has_collision(bullet, obstacle)) {
			bullet->destroy();
			if (obstacle->has_component<CHealth>()) {
				Obstacle* o = dynamic_cast<Obstacle*>(obstacle.get());
				if (!o) continue;
				o->damage();
			}
			else {
				obstacle->destroy();
			}
			bullet->destroy();
			return true;
		}
	}
	return false;
}

int PlayScene::get_score() const {
	if (player_->has_component<CScore>()) {
		return player_->get_component<CScore>().amount;
	}
	return 0;
}

void PlayScene::clear() {
	game_hud_.clear();
	enemy_system_.clear();
	//obstacles_spawner_.clear();
	health_bar_.clear();
}
