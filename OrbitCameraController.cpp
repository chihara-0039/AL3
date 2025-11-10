#define NOMINMAX
#include "OrbitCameraController.h"
#include <Windows.h>
#include <cmath>

static float clampf(float v, float a, float b) { return std::min(std::max(v, a), b); }
bool OrbitCameraController::KeyDown(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

void OrbitCameraController::Update(float dt) {
	// 右ドラッグ回転
	static bool dragging = false;
	static POINT last{};
	const bool rmb = KeyDown(VK_RBUTTON);

	if (rmb) {
		POINT p;
		GetCursorPos(&p);
		if (!dragging) {
			dragging = true;
			last = p;
		}
		float dx = float(p.x - last.x), dy = float(p.y - last.y);
		last = p;

		// 60fps基準正規化でfps非依存
		yawT -= dx * rotSpeed * (dt * 60.f);
		pitchT -= dy * rotSpeed * (dt * 60.f);
	} else {
		dragging = false;
	}

	// ズーム（暫定：PgUp/PgDn）
	if (KeyDown(VK_PRIOR))
		distT -= zoomStep;
	if (KeyDown(VK_NEXT))
		distT += zoomStep;

	// 制限
	pitchT = clampf(pitchT, minPitch, maxPitch);
	distT = clampf(distT, minDist, maxDist);

	// スムージング（一次遅れ）
	auto smooth = [](float cur, float tgt, float lam, float dt) {
		float a = 1.f - std::exp(-lam * dt);
		return cur + (tgt - cur) * a;
	};
	yaw = smooth(yaw, yawT, lambdaRot, dt);
	pitch = smooth(pitch, pitchT, lambdaRot, dt);
	dist = smooth(dist, distT, lambdaDist, dt);
}

void OrbitCameraController::GetBasis(Vec3& forward, Vec3& right) const {
	// 水平面上の前/右（移動に使う想定）
	float cy = std::cos(yaw), sy = std::sin(yaw), cp = std::cos(pitch);
	forward = {sy * cp, 0.0f, -cy * cp};
	right = {cy, 0.0f, sy};
	auto norm = [](Vec3 v) {
		float m = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		return (m > 1e-5f) ? Vec3{v.x / m, v.y / m, v.z / m} : Vec3{0, 0, 0};
	};
	forward = norm(forward);
	right = norm(right);
}

void OrbitCameraController::ComputeEyeAndRot(Vec3& outEye, Vec3& outRot) const {
	float cy = std::cos(yaw), sy = std::sin(yaw), cp = std::cos(pitch), sp = std::sin(pitch);
	Vec3 fwd{sy * cp, sp, -cy * cp};
	Vec3 look{target_.x, target_.y + 1.5f, target_.z};
	outEye = {look.x - fwd.x * dist, look.y - fwd.y * dist, look.z - fwd.z * dist};
	outRot = {pitch, yaw, 0.0f};
}
