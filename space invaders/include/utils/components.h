#pragma once

#include "utils/vec2.h"

using DWORD = unsigned long;

class Component
{
public:
	Component() = default;
	virtual ~Component() noexcept = default;
	Component(const Component&) noexcept = default;
	Component(Component&&) noexcept = default;
	Component& operator=(const Component&) noexcept = default;
	Component& operator=(Component&&) noexcept = default;
	bool has = false;
};

class CTransform : public Component {
public:
	Vec2 pos{ 0.0f, 0.0f };
	Vec2 size{ 1.0f, 1.0f };

	float angle = 0.0f;

	CTransform() = default;
	CTransform(Vec2 p) : pos(p) {}
	CTransform(Vec2 p, Vec2 s) : pos(p), size(s) {}
	CTransform(Vec2 p, Vec2 s, float a) : pos(p), size(s), angle(a) {}
};

class CLifeSpan : public Component {
public:
	size_t duration = 0;
	CLifeSpan() = default;
	CLifeSpan(size_t d) : duration(d) {}
};

class CScore : public Component {
public:
	size_t amount = 0;
	CScore() = default;
	CScore(size_t a) : amount(a) {};
};

class CCollision : public Component {
public:
	Vec2 local_pos{ 0.0f, 0.0f };
	float radius = 0.0f;
	CCollision() = default;
	CCollision(float r) : radius(r) {}
	CCollision(Vec2 l_pos, float r)
		: local_pos(l_pos), radius(r) {
	}
};

class CRenderer : public Component {
public:
	void* sprite = nullptr;
	DWORD color = 0xffffffff; // default white
	CRenderer() = default;
	CRenderer(void* s) : sprite(s) {}
	CRenderer(void* s, DWORD col) : sprite(s), color(col) {}
};

class CHealth : public Component {
	size_t max_hp_ = 3;
public:
	size_t current_hp = 0;
	CHealth() = default;
	CHealth(size_t max_h) : max_hp_(max_h), current_hp(max_h) {}
	size_t max_hp() { return max_hp_; }
};