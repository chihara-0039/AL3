#pragma once
#include "KamataEngine.h"
#include "skydome.h"

using namespace KamataEngine;

// とてもシンプルなクリアシーン：Spaceでタイトルへ
class GameClearScene {
public:
	~GameClearScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeFontMove = 2.0f;

	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* modelGameClear_ = nullptr;

	float counter_ = 0.0f;
	bool finished_ = false;

	float cheerTime_ = 0.0f;
	float basePlayerY_ = 0.0f;

	 // 追加：スカイドーム
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// 追加：暗転オーバーレイ
	uint32_t overlayTex_ = 0;
	Sprite* dimOverlay_ = nullptr;
};
