#pragma once
#include "Math.h"
#include <algorithm>

// 右ドラッグで回転 / ホイールでズーム（暫定PgUp/PgDn）
// プレイヤー位置を中心にオービットするコントローラ
class OrbitCameraController {
public:
	// 現在値
	float yaw = 0.0f;
	float pitch = 0.25f;
	float dist = 6.0f;

	// 目標値（入力はまずこちらを更新→スムージングで現在値へ）
	float yawT = yaw;
	float pitchT = pitch;
	float distT = dist;

	// 制限
	float minPitch = -1.2f, maxPitch = +1.2f;
	float minDist = 2.0f, maxDist = 12.0f;

	// 感度・スムージング
	float rotSpeed = 0.0030f; // ピクセル→ラジアン係数
	float zoomStep = 0.20f;   // 1ノッチの距離
	float lambdaRot = 18.0f;  // 回転追従の速さ[1/s]
	float lambdaDist = 18.0f; // 距離追従の速さ[1/s]

	// 追従対象（通常はプレイヤー）とロックオン
	void SetTarget(const Vec3& worldPos) { target_ = worldPos; }
	void SetLockOn(bool enabled, const Vec3* lockPos = nullptr) {
		lockOn_ = enabled;
		if (enabled && lockPos)
			lockPos_ = *lockPos;
	}

	// 入力→目標値→スムージング
	void Update(float dt);

	// 移動用の水平基底ベクトル（前/右）
	void GetBasis(Vec3& forward, Vec3& right) const;

	// エンジンCameraへ適用する Eye と Euler(X=pitch,Y=yaw,Z=0) を算出
	void ComputeEyeAndRot(Vec3& outEye, Vec3& outRot) const;

private:
	Vec3 target_{0, 0, 0};
	bool lockOn_ = false;
	Vec3 lockPos_{0, 0, 0};

	static bool KeyDown(int vk);
};
