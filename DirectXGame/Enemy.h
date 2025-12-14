#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class Enemy {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="camera">カメラ</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 被弾処理
	/// </summary>
	/// <param name="damage">与ダメージ</param>
	void OnHit(int damage);

	/// <summary>
	/// 死亡フラグ
	/// </summary>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 当たり判定用のAABB取得
	/// </summary>
	AABB GetAABB() const;

private:
	// ワールド変換
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル（必要なら使う）
	uint32_t textureHandle_ = 0u;

	// HP（ここではボスHP 60）
	int32_t hp_ = 60;

	// デスフラグ
	bool isDead_ = false;

	// 被弾フラッシュ用
	int32_t flashTimer_ = 0;
	static const int32_t kFlashDurationFrame = 6; // 被弾後6フレームだけ赤に

	// 当たり判定サイズ（適当に調整してOK）
	Vector3 halfSize_ = {2.0f, 2.0f, 2.0f};
};
