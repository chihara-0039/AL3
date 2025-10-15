#define NOMINMAX
#include <Windows.h>

#include "Player.h"
#include <cmath>
#include <cstring>
#include <algorithm>

//static float Clamp(float v, float a, float b) { return std::min(std::max(v, a), b); }

bool Player::KeyDown(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

bool Player::KeyPressed(int vk) {
	bool now = KeyDown(vk);
	bool was = keyLatch_[vk] != 0;
	keyLatch_[vk] = now ? 1 : 0;
	return now && !was;
}

void Player::Initialize(const Vec3& start) {
	pos_ = start;
	vel_ = {0, 0, 0};
	onGround_ = true;
	jumpCount_ = 0;
	dodging_ = false;
	dodgeTimer_ = 0.0f;
	dodgeCooldownTimer_ = 0.0f;
	reloading_ = false;
	reloadTimer_ = 0.0f;
	ammo_ = maxAmmo;
	std::memset(keyLatch_, 0, sizeof(keyLatch_));
}

void Player::Update(float dt, const Vec3& camF, const Vec3& camR) {
	// 入力処理
	HandleReload(dt);
	if (!reloading_) {
		HandleDodge(dt);
		HandleMovement(dt, camF, camR);
		HandleJumpAndGlide(dt);
	}

	// 位置更新
	pos_ += vel_ * dt;

	// 超簡易接地判定（y=0が地面）
	if (pos_.y <= 0.0f) {
		pos_.y = 0.0f;
		vel_.y = 0.0f;
		onGround_ = true;
		jumpCount_ = 0;
	} else {
		onGround_ = false;
	}

	// クールダウン
	if (dodgeCooldownTimer_ > 0.0f)
		dodgeCooldownTimer_ = std::max(0.0f, dodgeCooldownTimer_ - dt);
}

void Player::HandleMovement(float dt, const Vec3& camF, const Vec3& camR) {
	// カメラの水平成分を正規化
	Vec3 f = camF;
	f.y = 0.0f;
	Vec3 r = camR;
	r.y = 0.0f;
	auto norm = [](Vec3 v) {
		float m = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if (m < 1e-5f)
			return Vec3{0, 0, 0};
		return Vec3{v.x / m, v.y / m, v.z / m};
	};
	f = norm(f);
	r = norm(r);

	// 入力方向
	Vec3 wish{0, 0, 0};
	if (KeyDown('W'))
		wish += f;
	if (KeyDown('S'))
		wish += f * -1.0f;
	if (KeyDown('D'))
		wish += r;
	if (KeyDown('A'))
		wish += r * -1.0f;
	wish = norm(wish);

	// 目標水平速度（m/s）
	const float speed = onGround_ ? moveSpeed : (moveSpeed * airControl);
	const Vec3 targetVel = wish * speed;

	// 現在の水平速度
	Vec3 hv{vel_.x, 0.0f, vel_.z};

	// フレームレート非依存の一次遅れ追従：alpha = 1 - exp(-λ dt)
	// λ（応答速度）は好みで調整（地上速め/空中遅めにしてもよい）
	const float lambda = onGround_ ? 8.0f : 5.0f; // [1/s] 応答の速さ
	const float alpha = 1.0f - std::exp(-lambda * dt);

	hv += (targetVel - hv) * alpha;

	vel_.x = hv.x;
	vel_.z = hv.z;
}


void Player::HandleJumpAndGlide(float dt) {
	// ジャンプ（Space 立ち上がり）
	if (KeyPressed(VK_SPACE)) {
		if (onGround_) { // 1段目
			vel_.y = jumpVelocity;
			onGround_ = false;
			jumpCount_ = 1;
		} else if (jumpCount_ == 1) { // 2段目
			vel_.y = jumpVelocity;
			jumpCount_ = 2;
		}
	}

	// 重力 or 滑空
	bool gliding = (!onGround_) && KeyDown(VK_SPACE) && (vel_.y < 0.0f);
	float g = gravity * (gliding ? glideGravityScale : 1.0f);
	vel_.y -= g * dt;

	// 落下最高速度クランプ
	vel_.y = std::max(vel_.y, -50.0f);
}

void Player::HandleDodge(float dt) {
	// 回避入力（LeftShift）— クールダウン中でなく地上/空中どちらでもOK
	if (!dodging_ && dodgeCooldownTimer_ <= 0.0f && KeyPressed(VK_LSHIFT)) {
		// 入力方向がなければ前方へ
		Vec3 dir{0, 0, 0};
		if (KeyDown('W'))
			dir.z += 1;
		if (KeyDown('S'))
			dir.z -= 1;
		if (KeyDown('D'))
			dir.x += 1;
		if (KeyDown('A'))
			dir.x -= 1;
		float len = std::sqrt(dir.x * dir.x + dir.z * dir.z);
		if (len < 1e-4f)
			dir = {0, 0, 1};
		else {
			dir.x /= len;
			dir.z /= len;
		}

		dodging_ = true;
		dodgeTimer_ = dodgeTime;
		dodgeCooldownTimer_ = dodgeCooldown;
		dodgeDir_ = dir;
		// 瞬間的な水平速度付与
		vel_.x = dir.x * dodgeSpeed;
		vel_.z = dir.z * dodgeSpeed;
	}

	if (dodging_) {
		dodgeTimer_ -= dt;
		if (dodgeTimer_ <= 0.0f) {
			dodging_ = false;
		}
	}
}

void Player::HandleReload(float dt) {
	if (!reloading_ && KeyPressed('R')) {
		reloading_ = true;
		reloadTimer_ = reloadTime;
	}
	if (reloading_) {
		reloadTimer_ -= dt;
		if (reloadTimer_ <= 0.0f) {
			reloading_ = false;
			ammo_ = maxAmmo;
		}
	}
}

void Player::SwitchAttackMode() {
	int m = static_cast<int>(mode_);
	m = (m + 1) % static_cast<int>(AttackMode::Count);
	mode_ = static_cast<AttackMode>(m);
}

bool Player::TryConsumeAmmo() {
	if (reloading_)
		return false;
	if (ammo_ < burstCost)
		return false;
	ammo_ -= burstCost;
	return true;
}
