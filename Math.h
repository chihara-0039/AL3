#pragma once
#include <cmath>

struct Vec3 {
	float x = 0, y = 0, z = 0;

	Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
	Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
	Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
	Vec3& operator+=(const Vec3& o) {
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
};

inline float Length(const Vec3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
inline Vec3 Normalize(const Vec3& v) {
	float m = Length(v);
	if (m < 1e-6f)
		return {0, 0, 0};
	return {v.x / m, v.y / m, v.z / m};
}
inline Vec3 Cross(const Vec3& a, const Vec3& b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
inline float Clamp(float v, float a, float b) { return (v < a) ? a : (v > b) ? b : v; }
