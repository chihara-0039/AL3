#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

using namespace KamataEngine;

class Player {
public:
	/// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// <summary>
	/// 攻撃（スペースキー用）
	/// </summary>
	void Attack();

	// ★ 追加：狙い点へ撃つAPI（GameSceneから呼ぶ）
	void FireToward(const Vector3& targetWorld);

	/// プレイヤーのワールド座標（中心）取得
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 弾リスト（当たり判定用に参照させる）
	/// </summary>
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	Camera* camera_ = nullptr;

	// 移動速度
	Vector3 velocity_ = {0, 0, 0};

	// 弾
	std::list<PlayerBullet*> bullets_;

	// キーボード入力
	Input* input_ = nullptr;
};
