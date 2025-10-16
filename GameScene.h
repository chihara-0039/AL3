#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "OrbitCameraController.h"
#include "Player.h"

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	bool debugView_ = true; // 初期ONにしておくと最初から見えやすい
	int frameCounter_ = 0;

	float deltaTime_ = 1.0f / 60.0f;

	// ← エンジンの Camera を使う（Model::Draw がこれを要求するため）
	KamataEngine::Camera camera_;

	// 入力カメラ（オービット）
	OrbitCameraController camCtrl_;

	// プレイヤー
	Player player_;
	KamataEngine::Model* modelPlayer_ = nullptr;
	std::unique_ptr<KamataEngine::WorldTransform> wtPlayer_;

	// 床タイル
	std::vector<std::unique_ptr<KamataEngine::WorldTransform>> floorTiles_;
	int floorHalf_ = 6;      // 片側タイル数（合計 (2N+1)^2 枚）
	float floorStep_ = 2.0f; // タイル間隔
	float floorThin_ = 0.1f; // Yスケール（薄さ）

	bool KeyPressed(int vk);
};
