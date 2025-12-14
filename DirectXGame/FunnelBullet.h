#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

/// ファンネルから飛んでくるビーム弾（見た目は弾でもOK）
class FunnelBullet {
public:
	/// 初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// 更新
	void Update();

	/// 描画
	void Draw(const Camera& camera);

	bool IsDead() const { return isDead_; }
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_{};

	static const int32_t kLifeTime = 60 * 3; // 3秒くらいで消える
	int32_t lifeTimer_ = kLifeTime;
	bool isDead_ = false;
};
