#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

using namespace KamataEngine;

class Player {
public:
	/// 初期化
	void Initialize(Model* model, Model* bulletModel, Camera* camera, const Vector3& position);


	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// <summary>
	/// 攻撃（スペースキー用）
	/// </summary>
	void Attack();

	// 狙い点へ撃つAPI（GameSceneから呼ぶ）
	void FireToward(const Vector3& targetWorld);

	/// プレイヤーのワールド座標（中心）取得
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }
	// 当たり判定半径
	float GetCollisionRadius() const { return collisionRadius_; }

	//無敵状態か？
	bool IsInvincible() const { return invincibleTimer_ > 0; }
	//生きてるか？
	bool IsDead() const { return hp_ <= 0; }

	// 被弾処理
	void OnHit(int damage);

	/// <summary>
	/// 弾リスト（当たり判定用に参照させる）
	/// </summary>
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	

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

	//HP
	int32_t hp_ = 5;

	// 無敵タイマー
	int32_t invincibleTimer_ = 0;

	static const int32_t kInvincibleDuration = 60; // 無敵時間（60フレーム＝1秒）

	//プレイヤーの当たり判定の半径
	float collisionRadius_ = 1.0f;

	Model* bulletModel_ = nullptr;
};
