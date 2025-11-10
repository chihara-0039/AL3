#pragma once
#include <cmath>

// 3Dベクトル（全ファイルでこの定義を使う）
struct Vec3 {
	float x, y, z;
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

struct Mat4 {
	float m[16]; // 行優先の4x4行列
};

//単位行列
inline Mat4 MatIdentity() {
	Mat4 r;
	std::memset(r.m, 0, sizeof(r.m));
	r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
	return r;
}

// 右手系 LookAt
inline Mat4 MatLookAtRH(const Vec3& eye, const Vec3& at, const Vec3& upN) {
	Vec3 z = {eye.x - at.x, eye.y - at.y, eye.z - at.z}; // forward(視線反転)
	float zm = std::sqrt(z.x * z.x + z.y * z.y + z.z * z.z);
	if (zm < 1e-6f)
		z = {0, 0, 1};
	else {
		z.x /= zm;
		z.y /= zm;
		z.z /= zm;
	}
	// x = up × z
	Vec3 x = {upN.y * z.z - upN.z * z.y, upN.z * z.x - upN.x * z.z, upN.x * z.y - upN.y * z.x};
	float xm = std::sqrt(x.x * x.x + x.y * x.y + x.z * x.z);
	if (xm < 1e-6f)
		x = {1, 0, 0};
	else {
		x.x /= xm;
		x.y /= xm;
		x.z /= xm;
	}
	// y = z × x
	Vec3 y = {z.y * x.z - z.z * x.y, z.z * x.x - z.x * x.z, z.x * x.y - z.y * x.x};
	Mat4 r = MatIdentity();
	r.m[0] = x.x;
	r.m[1] = y.x;
	r.m[2] = z.x;
	r.m[3] = 0.0f;
	r.m[4] = x.y;
	r.m[5] = y.y;
	r.m[6] = z.y;
	r.m[7] = 0.0f;
	r.m[8] = x.z;
	r.m[9] = y.z;
	r.m[10] = z.z;
	r.m[11] = 0.0f;
	r.m[12] = -(x.x * eye.x + x.y * eye.y + x.z * eye.z);
	r.m[13] = -(y.x * eye.x + y.y * eye.y + y.z * eye.z);
	r.m[14] = -(z.x * eye.x + z.y * eye.y + z.z * eye.z);
	r.m[15] = 1.0f;
	return r;
}

// 右手系 PerspectiveFov
inline Mat4 MatPerspectiveFovRH(float fovY, float aspect, float zn, float zf) {
	Mat4 r;
	std::memset(r.m, 0, sizeof(r.m));
	const float y = 1.0f / std::tan(fovY * 0.5f);
	const float x = y / aspect;
	r.m[0] = x;
	r.m[5] = y;
	r.m[10] = zf / (zn - zf);
	r.m[11] = -1.0f;
	r.m[14] = (zn * zf) / (zn - zf);
	return r;
}
