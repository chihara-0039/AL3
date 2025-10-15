#define NOMINMAX
#include "GameScene.h"
#include <Windows.h>
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace KamataEngine;

// ======== ユーティリティ ========
static bool KeyDown(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

// ======== SimpleCamera ========
void SimpleCamera::Update(float dt) {
	// 右クリックでロックオン（トグル）
	static bool rightLatch = false;
	const bool rightNow = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
	if (rightNow && !rightLatch) {
		lockOn = !lockOn;
	}
	rightLatch = rightNow;

	// 右ドラッグで回転
	static bool dragging = false;
	static POINT last{};
	if (rightNow) {
		POINT p;
		GetCursorPos(&p);
		if (!dragging) {
			dragging = true;
			last = p;
		}
		float dx = float(p.x - last.x);
		float dy = float(p.y - last.y);
		last = p;

		// 60fps基準で正規化：フレームレート非依存
		const float rotSpeed = 0.003f * (dt * 60.0f);
		yaw -= dx * rotSpeed;
		pitch -= dy * rotSpeed;
		pitch = std::clamp(pitch, -1.2f, 1.2f);
	}else{
		dragging = false;
	}

	// ホイールで距離（単純化：PageUp/Downで代用）
	const float zoomStep = 0.10f * (dt * 60.0f);
	if (GetAsyncKeyState(VK_PRIOR) & 0x8000)
		dist -= zoomStep; // PageUp
	if (GetAsyncKeyState(VK_NEXT) & 0x8000)
		dist += zoomStep; // PageDown
	dist = std::clamp(dist, 2.5f, 12.0f);
}

void SimpleCamera::GetBasis(Vec3& forward, Vec3& right) const {
	float cy = std::cos(yaw), sy = std::sin(yaw);
	float cp = std::cos(pitch), sp = std::sin(pitch);
	// カメラの前方（プレイヤーから見た前）
	forward = {sy * cp, sp, -cy * cp};
	// 右
	right = {cy, 0.0f, sy};
}

// ======== GameScene ========
bool GameScene::KeyPressed(int vk) {
	static uint8_t latch[256] = {};
	bool now = KeyDown(vk);
	bool was = latch[vk] != 0;
	latch[vk] = now ? 1 : 0;
	return now && !was;
}

void GameScene::Initialize() {
	// プレイヤー初期化
	player_.Initialize({0, 0, 0});

	// カメラ(エンジン側)
	camera_.Initialize();
	camera_.translation_ = {0.0f, 3.0f, -8.0f};
	camera_.rotation_ = {0.2f, 0.0f, 0.0f};
	camera_.UpdateMatrix();


}

void GameScene::Update() {
	// カメラ更新
	cam_.Update(deltaTime_);

	// カメラ基底
	Vec3 camF, camR;
	cam_.GetBasis(camF, camR);

	// 攻撃モード切替（C）
	if (KeyPressed('C')) {
		player_.SwitchAttackMode();
	}

	// 左クリックで発射（今回は消費のみ。弾生成は後段で実装）
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
		if (player_.TryConsumeAmmo()) {
			// TODO: 実弾生成（弾種に応じてエンティティ生成）
			// ・Straight: まっすぐ前へ
			// ・Homing  : 追尾
			// ・Laser   : 貫通Ray
			// ・Charge  : チャージ段階別威力
		}
	}

	// プレイヤー更新
	player_.Update(deltaTime_, camF, camR);

	// （描画カメラへの反映は Draw() で実施）
}

void GameScene::Draw() {
	// ===== ここではデバッグ用のHUD出力例を記述 =====
	

	

	
}
