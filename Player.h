#pragma once
#include "Math.h"
#include <algorithm>
#include <cstdint>

// 弾モード
enum class AttackMode : int { Straight = 0, Homing, Laser, ChargeShot, Count };

class Player {
public:
	// 調整しやすい公開パラメータ
	float moveSpeed = 8.0f, airControl = 0.6f, gravity = 25.0f, jumpVelocity = 9.5f, glideGravityScale = 0.22f;
	float dodgeSpeed = 18.0f, dodgeTime = 0.20f, dodgeCooldown = 0.75f;

	int maxAmmo = 12, burstCost = 1;
	float reloadTime = 1.8f;

	// 状態取得
	const Vec3& Position() const { return pos_; }
	const Vec3& Velocity() const { return vel_; }
	bool OnGround() const { return onGround_; }
	AttackMode Mode() const { return mode_; }
	int Ammo() const { return ammo_; }
	bool IsReloading() const { return reloading_; }

	// ライフサイクル
	void Initialize(const Vec3& start = {0, 0, 0});
	void Update(float dt, const Vec3& camForward, const Vec3& camRight);
	void SwitchAttackMode();
	bool TryConsumeAmmo();

private:
	// 入力ヘルパ
	static bool KeyDown(int vk);
	bool KeyPressed(int vk); // 立ち上がり検出

	// 内部更新
	void HandleMovement(float dt, const Vec3& camF, const Vec3& camR);
	void HandleJumpAndGlide(float dt);
	void HandleDodge(float dt);
	void HandleReload(float dt);

	// 状態
	Vec3 pos_{0, 0, 0};
	Vec3 vel_{0, 0, 0};
	bool onGround_ = true;
	int jumpCount_ = 0;

	// 回避
	float dodgeTimer_ = 0.0f, dodgeCooldownTimer_ = 0.0f;
	bool dodging_ = false;
	Vec3 dodgeDir_{0, 0, 0};

	// リロード
	bool reloading_ = false;
	float reloadTimer_ = 0.0f;
	int ammo_ = 0;

	// 入力立ち上がり検出
	uint8_t keyLatch_[256] = {};

	AttackMode mode_ = AttackMode::Straight;
};
