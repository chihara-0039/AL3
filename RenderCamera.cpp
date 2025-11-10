#include "RenderCamera.h"
#include <cmath>

void RenderCamera::BuildVP() {
	// yaw/pitch から注視点を作る（eye + forward）
	const float cy = std::cos(yaw), sy = std::sin(yaw);
	const float cp = std::cos(pitch), sp = std::sin(pitch);
	Vec3 fwd{sy * cp, sp, -cy * cp};
	Vec3 at{eye.x + fwd.x, eye.y + fwd.y, eye.z + fwd.z};

	vp_.view = MatLookAtRH(eye, at, {0, 1, 0});
	vp_.proj = MatPerspectiveFovRH(fovY, aspect, nearZ, farZ);

	// viewProj（必要なら）
	Mat4 vp = MatIdentity();
	// 行列積（行優先なら v * p）
	// vp = view * proj
	const float* a = vp_.view.m;
	const float* b = vp_.proj.m;
	float* c = vp.m;
	for (int r = 0; r < 4; ++r) {
		for (int k = 0; k < 4; ++k) {
			c[r * 4 + 0] += a[r * 4 + 0] * b[k + 0];
			c[r * 4 + 1] += a[r * 4 + 1] * b[k + 4];
			c[r * 4 + 2] += a[r * 4 + 2] * b[k + 8];
			c[r * 4 + 3] += a[r * 4 + 3] * b[k + 12];
		}
	}
	vp_.viewProj = vp;
}