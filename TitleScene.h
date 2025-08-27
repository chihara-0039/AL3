#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "skydome.h"

using namespace KamataEngine;

// タイトルシーン
class TitleScene {
public:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,   // フェードイン
		kMain,     // メイン部（待機）
		kStartRun, // ★スタート演出（走り抜け）
		kFadeOut,  // フェードアウト
	};

	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeTitleMove = 2.0f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* modelTitle_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	float counter_ = 0.0f;
	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	// --- 走り抜け演出用 ---
	float runTime_ = 0.0f;                         // 経過時間
	static inline const float kRunDuration = 1.2f; // 走り抜け時間（秒）
	float basePlayerY_ = 0.0f;                     // プレイヤーの基準Y
	float runXStart_ = -40.0f;                     // 左端（画面外くらい）
	float runXEnd_ = 40.0f;                        // 右端（画面外くらい）

	// イージング
	static inline float EaseOutCubic(float t) { return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t); }
};
