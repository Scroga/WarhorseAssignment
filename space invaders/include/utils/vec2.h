#pragma once
#include <iostream>
#include <cmath>

template <typename T>
class Vector2
{
public:
	T x = 0;
	T y = 0;

	constexpr Vector2() = default;
	constexpr Vector2(T xin, T yin) : x(xin), y(yin) {}
	constexpr explicit Vector2(T v) : x(v), y(v) {}
	Vector2(const Vector2&) = default;
	Vector2(Vector2&&) = default;
	Vector2<T>& operator=(const Vector2<T>&) = default;
	Vector2<T>& operator=(Vector2<T>&&) noexcept = default;

	bool operator==(const Vector2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Vector2<T>& rhs) const { return x != rhs.x || y != rhs.y; }

	Vector2<T> operator+(const Vector2<T>& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2<T> operator-(const Vector2<T>& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2<T> operator*(const T value) const { return Vector2(x * value, y * value); }
	Vector2<T> operator/(const T value) const { return Vector2(x / value, y / value); }

	void operator-() { x *= -1; y *= -1; }
	void operator+=(const Vector2<T>& rhs) { x += rhs.x; y += rhs.y; }
	void operator-=(const Vector2<T>& rhs) { x -= rhs.x; y -= rhs.y; }

	void operator+=(const T value) { x += value; y += value; }
	void operator-=(const T value) { x -= value; y -= value; }
	void operator*=(const T value) { x *= value; y *= value; }
	void operator/=(const T value) { x /= value; y /= value; }

	friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector) {
		os << vector.x << ' ' << vector.y;
		return os;
	}

	float distance(const Vector2<T>& rhs) const {
		return std::sqrt((rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y));
	}

	float length() const { return std::sqrt(x * x + y * y); }

	void noramlize() {
		float len = length();
		if (len != 0) x /= len; y /= len;
	}
};

using Vec2 = Vector2<float>;
using Vec2i = Vector2<int>;
using Vec2u = Vector2<unsigned int>;
