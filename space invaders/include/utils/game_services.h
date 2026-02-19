#pragma once

#include <d3d9.h>
#include <memory>

#include "systems/input_handler.h"
#include "systems/sprite_renderer.h"
#include "systems/sound_handler.h"
#include "systems/entity_manager.h"
#include "systems/asset_manager.h"

struct GameServices
{
	const Vec2 window_size;
	SpriteRenderer sprite_renderer;
	SoundHandler sound_handler{};
	InputHandler& input_handler;
	EntityManager entity_manager{};
	AssetManager asset_manager;

	GameServices(LPDIRECT3DDEVICE9 pd3dDevice, InputHandler& input_h, Vec2 w_size)
		: sprite_renderer(pd3dDevice)
		, asset_manager(sprite_renderer)
		, input_handler(input_h)
		, window_size(w_size) {
	}

	GameServices(const GameServices&) = delete;
	GameServices& operator=(const GameServices&) = delete;
};