#pragma once
#include "KamataEngine.h"

// ゲームシーン
class GameScene {
public:

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	//3Dモデル
	KamataEngine::Model* model_ = nullptr;

};