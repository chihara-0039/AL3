// Enemy.h

#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include <list>

// 前方宣言
class FunnelBullet;

using namespace KamataEngine;

class Enemy {
public:
	// 行動パターンの定義
	enum class Behavior {
		kRoot,   // 待機・基本姿勢
		kAttack, // 攻撃
		kMove,   // 移動
	};

	int GetHP() const { return hp_; }
	static constexpr int kMaxHP = 60;

	// 初期化に弾モデル（bulletModel）を追加
	void Initialize(Model* model, Model* bulletModel, Camera* camera, const Vector3& position);

	// Updateで弾リストを受け取る形に変更
	void Update(std::list<FunnelBullet*>& bullets);

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }


	void Draw();
	void OnHit(int damage);
	bool IsDead() const { return isDead_; }
	AABB GetAABB() const;

private:
	// 敵の攻撃行動（内部関数）
	void FireSpreadShot(std::list<FunnelBullet*>& bullets);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr; // 弾モデル
	Camera* camera_ = nullptr;

	int hp_ = kMaxHP;
	bool isDead_ = false;

	int flashTimer_ = 0;
	static const int kFlashDurationFrame = 6;
	Vector3 halfSize_{2.0f, 2.0f, 2.0f};
	Vector3 basePos_{};


	// 行動制御用
	Behavior behavior_ = Behavior::kRoot;
	Behavior prevBehavior_ = Behavior::kRoot;
	int behaviorTimer_ = 0; // 行動の経過時間

	// パラメータ
	float kWalkSpeed = 0.1f;
};