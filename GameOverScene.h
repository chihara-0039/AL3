#pragma once
#include "KamataEngine.h"
#include "Fade.h"
#include "skydome.h" 

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

	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// フェード制御
	Fade* fade_ = nullptr;
	bool requestExit_ = false;	//退出（タイトルへ戻る）要求フラグ
};