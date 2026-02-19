#pragma once
#include <cmath>

#include "utils/custom_vertex.h"
#include "game_clock.h"
#include "vec2.h"

#define PI ((float)3.1415926535)

inline constexpr float deg_to_rad(float deg) { return deg * (PI / 180.0f); }

inline float bobbing(const GameClock& clock, float pos, float freq, float scale) {
	float bobbing = std::sin(clock.get_frames_count() * freq) * scale;
	return pos + bobbing * clock.get_delta_time();
}

inline DWORD change_color(DWORD old_color, int i) {
    // Extract channels
    unsigned int a = (old_color >> 24) & 0xFF;
    unsigned int r = (old_color >> 16) & 0xFF;
    unsigned int g = (old_color >> 8) & 0xFF;
    unsigned int b = (old_color >> 0) & 0xFF;

    // Using different increments per channel helps avoid similar colors
    unsigned int dr = (unsigned int)(i * 37u);
    unsigned int dg = (unsigned int)(i * 71u);  
    unsigned int db = (unsigned int)(i * 97u);

    // Add and wrap to 0..255
    r = (r + dr) & 0xFF;
    g = (g + dg) & 0xFF;
    b = (b + db) & 0xFF;

    // Pack back
    return (a << 24) | (r << 16) | (g << 8) | (b);
}

inline Vec2 rotate_point(Vec2 center, Vec2 offset, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Vec2( 
        center.x + offset.x * c + offset.y * s,
        center.y + offset.y * c - offset.x * s );
}


////////////////////////////////////////////////////

inline float clamp(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// percent in [0..100], screen in pixels -> pixels
inline Vec2 percentPosToPixels(Vec2 percentPos01to100, Vec2 screenSizePx) {
    const float px = clamp(percentPos01to100.x, 0.f, 100.f) / 100.f;
    const float py = clamp(percentPos01to100.y, 0.f, 100.f) / 100.f;

    return {
        px * screenSizePx.x,
        py * screenSizePx.y
    };
}

// percent in [0..100] of screen size, screen in pixels -> pixels
inline Vec2 percentSizeToPixels(Vec2 percentSize01to100, Vec2 screenSizePx) {
    const float sx = clamp(percentSize01to100.x, 0.f, 100.f) / 100.f;
    const float sy = clamp(percentSize01to100.y, 0.f, 100.f) / 100.f;

    return {
        sx * screenSizePx.x,
        sy * screenSizePx.y
    };
}