#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Player {
public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, Camera* camera);
	// 更新
	void Update();
	// 描画
	void Draw();
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

private: // C++ではメンバ変数は特別な理由がなければprivateにする
	// ワールド変換データ
	WorldTransform worldTransform;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle = 0u;
	// カメラ
	Camera* camera_ = nullptr;
};
