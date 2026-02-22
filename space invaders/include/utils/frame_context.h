#pragma once

#include "utils/vec2.h"
#include "utils/game_clock.h"

struct FrameContext
{
	const Vec2 window_size;
	const GameClock& clock;

	FrameContext(Vec2 w_size, const GameClock& c)
		: window_size(w_size)
		, clock(c) {
	}
};