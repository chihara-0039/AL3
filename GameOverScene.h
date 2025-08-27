#pragma once
#include "KamataEngine.h"
#include "Fade.h"

using namespace KamataEngine;

// 02_12 19枚目 タイトルシーン
class GameOverScene {
public:
	~GameOverScene();

	void Initialize();

	void Update();

	void Draw();

	// 02_12 26枚目
	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeFontMove = 2.0f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* modelGameOver_ = nullptr;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// フェード制御
	Fade* fade_ = nullptr;
	bool requestExit_ = false;	//退出（タイトルへ戻る）要求フラグ

	//黒背景用スプライト
	uint32_t overlayTex_ = 0;
	Sprite* blackOverlay_ = nullptr;
};