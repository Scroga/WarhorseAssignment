#pragma once
#include <cmath>

#include "utils/custom_vertex.h"
#include "game_clock.h"
#include "vec2.h"

#define PI ((float)3.1415926535)

/// Converts an angle from degrees to radians.
/// @param deg Angle in degrees.
/// @return Angle in radians.
inline constexpr float deg_to_rad(float deg) { return deg * (PI / 180.0f); }

/// Computes a simple sinusouidal "bobbing".
/// @param clock Provides the current frame count and delta time.
/// @param pos Base position value (x or y) to bob around.
/// @param freq Bobbing frequency factor (higher = faster oscillation).
/// @param scale Bobbing amplitude (higher = bigger displacement).
/// @return New position with bobbing applied.
inline float bobbing(const GameClock& clock, float pos, float freq, float scale) {
	float bobbing = std::sin(clock.get_frames_count() * freq) * scale;
	return pos + bobbing * clock.get_delta_time();
}

/// Returns a point moving along an oval (ellipse) centered at center.
/// @param ceneter Ceneter of the oval (ellipse).
/// @param oval_size Width and height of oval.
/// @param clock Game clock used as the time source (frames count).
/// @param angular_speed Angular speed in radians per frame (higher = faster).
/// @param phase Phase offset in radians (shifts the starting point).
/// @return New position on the oval for the current time.
inline Vec2 oval_trajectory(Vec2 center, Vec2 oval_size, const GameClock& clock, float angular_speed = 1.0f, float phase = deg_to_rad(180.0f)) {
    const float a = oval_size.x * 0.5f;   // x radius
    const float b = oval_size.y * 0.5f;  // y radius

    const float angle = phase + clock.get_frames_count() * angular_speed;

    return Vec2{
        center.x + a * std::cos(angle),
        center.y + b * std::sin(angle)
    };
}

/// Changes RGB channels of a color using index based offset.
/// Each color channel is incremented by different multiplier.
/// Alpha channel remains unchanged.
/// @param old_color Original color in ARGB format (0xAARRGGBB).
/// @param i Index used to generate channel offsets.
/// @return New color in ARGB format with modified RGB channels.
inline DWORD offset_color(DWORD old_color, int i) {
    // extract channels
    unsigned int a = (old_color >> 24) & 0xFF;
    unsigned int r = (old_color >> 16) & 0xFF;
    unsigned int g = (old_color >> 8) & 0xFF;
    unsigned int b = (old_color >> 0) & 0xFF;

    // using different increments per channel helps avoid similar colors
    unsigned int dr = (unsigned int)(i * 37u);
    unsigned int dg = (unsigned int)(i * 71u);  
    unsigned int db = (unsigned int)(i * 97u);

    // add and wrap to 0..255
    r = (r + dr) & 0xFF;
    g = (g + dg) & 0xFF;
    b = (b + db) & 0xFF;

    // pack back
    return (a << 24) | (r << 16) | (g << 8) | (b);
}

/// Rotates a point around `center` by `angle` radians.
/// The point is provided as an `offset` from the center.
/// @param center Center of rotation.
/// @param offset Local offset from the center to rotate.
/// @param angle Rotation angle in radians (positive = counter-clockwise).
/// @return Rotated point in world coordinates.
inline Vec2 rotate_point(Vec2 center, Vec2 offset, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);

    return Vec2( 
        center.x + offset.x * c + offset.y * s,
        center.y + offset.y * c - offset.x * s );
}

/// Converts a position given in percent of the screen into pixels
/// @param percent Vec2 in percent of screen (0..100)
/// @param screenSize Screen size in pixels
/// @return Vec2 position in pixels
inline Vec2 pos_to_px(Vec2 percent, Vec2 screen_size) {
    const Vec2 p = percent / 100.0f;
    return Vec2{ p.x * screen_size.x, p.y * screen_size.y };
}

/// Converts screen position given in pixels into 
/// a position given in percent of the sceen
/// 
/// Percent values are clamped to [0, 100]
/// @param  pos Vec2 position in pixels
/// @param screenSize Screen size in pixels
/// @return Vec2 position in percent of the screen
inline Vec2 px_to_pos(Vec2 pos, Vec2 screen_size) {
    const Vec2 p(pos.x / screen_size.x, pos.y / screen_size.y);
    return p * 100.0f;
}

/// Converts a size given in percent of the screen into pixels
/// 
/// Percent values are clamped to [0, 100]
/// @param  percent Vec2 in percent of screen (0..100)
/// @param screenSize Screen size in pixels
/// @return Vec2 size of sprite in pixels
inline Vec2 size_to_px(Vec2 percent, Vec2 screen_size) {
    const Vec2 p = percent / 100.0f;
    float max = screen_size.x > screen_size.y ? screen_size.x : screen_size.y;
    return Vec2(p * max);

}

/// Converts a size given in percent of the screen into pixels
///
/// Percent values are clamped to [0, 100]
/// @param  percent Float in percent of screen (0..100)
/// @param screenSize Screen size in pixels
/// @return float size of sprite in pixels
inline float size_to_px(float percent, Vec2 screen_size) {
    const float p = percent/ 100.0f;
    float max = screen_size.x > screen_size.y ? screen_size.x : screen_size.y;
    return p * max;

}