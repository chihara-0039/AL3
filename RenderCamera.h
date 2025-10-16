#pragma once
#include "Math.h"

// ゲーム側カメラ（行列を保持してエンジンへ渡す）
struct ViewProjection {
	Mat4 view;
	Mat4 proj;
	Mat4 viewProj; // 必要なら
};

class RenderCamera {
public:
	// 状態
	Vec3 eye{0, 2.5f, -6.5f};
	float yaw = 0.0f, pitch = 0.25f; // 回転はオービット側で保持してもOK
	float fovY = 60.0f * 3.1415926f / 180.0f;
	float nearZ = 0.05f, farZ = 1000.0f;
	float aspect = 16.0f / 9.0f;

	// 目標位置と向き（OrbitCameraController から渡す）
	void SetFromOrbit(const Vec3& eyePos, float pitch_, float yaw_) {
		eye = eyePos;
		pitch = pitch_;
		yaw = yaw_;
	}

	// 行列を組む
	void BuildVP();

	const ViewProjection& GetVP() const { return vp_; }

private:
	ViewProjection vp_;
};