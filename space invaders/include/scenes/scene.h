#pragma once

#include "utils/game_clock.h"
#include "utils/game_services.h"
#include "utils/game_hud.h"
#include "scenes/scene_type.h"

class Scene {
protected:
	GameHud game_hud_;
	GameServices& services_;
	bool has_ended_ = false;
public:
	const SceneType type;
	explicit Scene(SceneType t, GameServices& services) : type(t), services_(services) {}
	virtual ~Scene() = default;

	virtual void setup_hud() = 0;
	virtual void draw_entities() = 0;
	virtual void handle_input(const GameClock& clock) = 0;
	virtual void update(const GameClock& clock) = 0;
	virtual void clear() = 0;
	
	bool has_ended() { return has_ended_; }
	void end_scene() { has_ended_ = true; }
};