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

inline float clamp(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

inline Vec2 clamp(Vec2 v, float lo, float hi) {
    return Vec2(clamp(v.x, lo, hi), clamp(v.y, lo, hi));
}

///
/// @param percent Vector in range [0, 100] (clamp will be applied)
/// @param screenSize Current size of the screen
/// @return 
inline Vec2 percent_to_pixels(Vec2 percent, Vec2 screenSize) {
    const Vec2 p = clamp(percent, 0.0f, 100.f) / 100.0f;
    return Vec2{ p.x * screenSize.x, p.y * screenSize.y };
}