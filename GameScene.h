#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	~GameScene();

private:
	// テクスチャハンドル
	uint32_t textureHandle = 0;

	// 3Dモデルデータ
	Model* model = nullptr;
	

	// カメラ
	Camera camera;

	// 自キャラ
	Player* player_ = nullptr;

	// 3Dモデルデータ(block)AL3_02_02
	Model* modelBlock = nullptr;

	// 3Dモデルデータ(skydome)
	Model* modelSkydome_ = nullptr;

	// ブロック用のワールドトランスフォームAL3_0202
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks;
	// 天球
	Skydome* skydome_ = nullptr;

	// デバックカメラ有効AL3_02_02
	bool isDebugCameraActive = false;

	// デバッグカメラAL3_0202
	DebugCamera* debugCamera = nullptr;

	//数学関数
	Math math_;
};