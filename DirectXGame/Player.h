#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

using namespace KamataEngine;

class Player {
public:
	// ===== HP HUD 用（public）=====
	int32_t GetHP() const { return hp_; }
	static constexpr int32_t kMaxHP = 5;


	void Initialize(Model* model, Model* bulletModel, Camera* camera, const Vector3& position);
	void Update();
	void Draw();

	void Attack();
	void FireToward(const Vector3& targetWorld);

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }
	float GetCollisionRadius() const { return collisionRadius_; }

	bool IsInvincible() const { return invincibleTimer_ > 0; }
	bool IsDead() const { return hp_ <= 0; }

	void OnHit(int damage);

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	~Player();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	Camera* camera_ = nullptr;

	std::list<PlayerBullet*> bullets_;
	Input* input_ = nullptr;

	Vector3 velocity_{};

	int hp_ = 5;

	int invincibleTimer_ = 0;
	static const int kInvincibleDuration = 60;

	float collisionRadius_ = 1.0f;

	// 自分の座標を教える関数
	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }
};
