#pragma once
#include "KamataEngine.h"
#include <cstdint>
#include <memory>

class GameScene {
public:
	~GameScene();
	void Initialize();
	void Update();
	void Draw();

private:
	// テクスチャ
	uint32_t textureHandle_ = 0;

	// モデル（1つ表示するだけの最小）
	KamataEngine::Model* model_ = nullptr;

	// 各オブジェクト用：ワールド変換（今回は1つ）
	std::unique_ptr<KamataEngine::WorldTransform> worldTransform_;

	// シーンで共有：ビュープロジェクション（カメラ）
	KamataEngine::ViewProjection viewProjection_;

	// ちょい回転させて動きを確認
	float angleY_ = 0.0f;

	// 立ち上がり検出
	bool KeyPressed(int vk);
};
