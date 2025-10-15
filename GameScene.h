#pragma once
#include "KamataEngine.h"
#include "Player.h"

using namespace KamataEngine;

// 簡易カメラ（右ドラッグで回転、ホイールで距離）＋右クリックでロックオン準備フラグ
struct SimpleCamera {
	float yaw = 0.0f;   // 水平回転
	float pitch = 0.2f; // 上下回転
	float dist = 6.0f;  // プレイヤーからの距離
	bool lockOn = false;

	// 画面中心に相対したマウス差分で回す
	void Update(float dt);

	// 前方・右ベクトル（ワールド）
	void GetBasis(struct Vec3& forward, struct Vec3& right) const;
};

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	// 時間
	float deltaTime_ = 1.0f / 60.0f; // 単純な固定タイムステップ

	//カメラ
	Camera camera_;
	SimpleCamera cam_;

	// プレイヤー
	Player player_;
	Model* modelPlayer_ = nullptr;
	std::unique_ptr<WorldTransform> wtPlayer_;
	
	// ===グリッド線（線orタイル) ===
	bool useLineGrid_ = true; // 線グリッド or タイルグリッド

	Model* modelGridLine_ = nullptr; // 線グリッド
	std::vector<std::unique_ptr<KamataEngine::WorldTransform>> tiles_;
	int gridHalfCount_ = 12; // 片側タイル数
	float tileSize_ = 2.0f;  // タイル1枚の一辺[m]


	// 入力立ち上がり（C：攻撃モード切替、左クリック：射撃）
	bool KeyPressed(int vk);

	// 内部：マウス情報
	POINT prevMouse_ = {0, 0};
	bool mouseLatch_[2] = {false, false};
};
