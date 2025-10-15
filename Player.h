#pragma once
#include <algorithm>
#include <cstdint>

// 3Dベクトルの極小ユーティリティ（必要最小限）
struct Vec3 {
	float x, y, z;
	Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
	Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
	Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
	Vec3& operator+=(const Vec3& o) {
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
};

// 攻撃モード
enum class AttackMode : int {
	Straight = 0, // ストレート弾
	Homing,       // ホーミング
	Laser,        // レーザー
	ChargeShot,   // チャージ
	Count
};

class Player {
public:
	// 公開パラメータ（ゲームバランス調整しやすく）
	float moveSpeed = 8.0f;          // 地上移動速度 [m/s]
	float airControl = 0.6f;         // 空中制御の割合
	float gravity = 25.0f;           // 重力 [m/s^2]
	float jumpVelocity = 9.5f;       // 1段/2段ジャンプ初速
	float glideGravityScale = 0.22f; // 滑空時の重力係数
	float dodgeSpeed = 18.0f;        // 緊急回避の初速
	float dodgeTime = 0.20f;         // 回避モーション時間 [s]
	float dodgeCooldown = 0.75f;     // 回避のクールダウン [s]

	// 弾薬
	int maxAmmo = 12;
	int burstCost = 1;       // 1発消費（弾種で可変に予定）
	float reloadTime = 1.8f; // Rキーでのリロード時間

	// 状態取得
	const Vec3& Position() const { return pos_; }
	const Vec3& Velocity() const { return vel_; }
	bool OnGround() const { return onGround_; }
	AttackMode Mode() const { return mode_; }
	int Ammo() const { return ammo_; }
	bool IsReloading() const { return reloading_; }

	// 初期化
	void Initialize(const Vec3& start = {0, 0, 0});

	// 方向（カメラ前方・右）と入力で更新
	void Update(float dt, const Vec3& camForward, const Vec3& camRight);

	// 攻撃モード切替（C）
	void SwitchAttackMode();

	// 発射要求（左クリック）→ 実際の弾生成は外で。ここでは弾消費と可否だけ。
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
	int jumpCount_ = 0; // 0=地上、1=1段目消費、2=2段目消費

	// 回避
	float dodgeTimer_ = 0.0f;
	float dodgeCooldownTimer_ = 0.0f;
	bool dodging_ = false;
	Vec3 dodgeDir_{0, 0, 0};

	// リロード
	bool reloading_ = false;
	float reloadTimer_ = 0.0f;
	int ammo_ = 0;

	// 入力立ち上がり検出
	uint8_t keyLatch_[256] = {};

	// 攻撃モード
	AttackMode mode_ = AttackMode::Straight;
};
