#include "Enemy.h"
#include "FunnelBullet.h" 
#include <cassert>
#include <cmath>
#include <algorithm>

void Enemy::Initialize(Model* model, Model* bulletModel, Camera* camera, const Vector3& position) {
	assert(model);
	assert(bulletModel);
	assert(camera);

	model_ = model;
	bulletModel_ = bulletModel;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};

	// 基準位置を保存
	basePos_ = position;


	// 最初は待機状態から
	behavior_ = Behavior::kRoot;
	behaviorTimer_ = 0;

	WorldTransformUpdate(worldTransform_);
}

void Enemy::Update(std::list<FunnelBullet*>& bullets) {
	if (isDead_) {
		return;
	}

	// 被弾フラッシュ
	if (flashTimer_ > 0) {
		--flashTimer_;
	}

	// HPが半分以下なら発狂モード（動作倍速）
	const int speedMultiplier = (hp_ < kMaxHP / 2) ? 2 : 1;

	// 揺れパラメータ
	const float freq = 0.05f;
	const float idleAmpY = 0.6f;
	const float moveAmpY = 0.8f;

	// 行動タイマー進行
	behaviorTimer_++;

	switch (behavior_) {
	case Behavior::kRoot: {
		// 基準位置 + sin（漂流しない）
		worldTransform_.translation_.y = basePos_.y + std::sin(behaviorTimer_ * freq) * idleAmpY;
		worldTransform_.translation_.x = basePos_.x; // Root中はX固定（好みで）

		if (behaviorTimer_ >= 60 / speedMultiplier) {
			// 次状態の基準を「今の位置」に更新（ワープ防止）
			basePos_ = worldTransform_.translation_;
			behavior_ = Behavior::kAttack;
			behaviorTimer_ = 0;
		}
	} break;

	case Behavior::kAttack: {
		FireSpreadShot(bullets);

		// Move開始時の基準を「今の位置」にする
		basePos_ = worldTransform_.translation_;
		behavior_ = Behavior::kMove;
		behaviorTimer_ = 0;
	} break;

	case Behavior::kMove: {
		// 「今いる位置(basePos_) を中心に」8の字っぽく動く（代入式で統一）
		worldTransform_.translation_.x = basePos_.x + std::cos(behaviorTimer_ * freq) * 3.0f;
		worldTransform_.translation_.y = basePos_.y + std::sin(behaviorTimer_ * freq) * moveAmpY;

		if (behaviorTimer_ >= 120 / speedMultiplier) {
			// Rootへ戻るときも基準更新（ワープ防止）
			basePos_ = worldTransform_.translation_;
			behavior_ = Behavior::kRoot;
			behaviorTimer_ = 0;
		}
	} break;
	}

	// 画面外保険（std::clampが使える前提：<algorithm> + C++17）
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -3.0f, 8.0f);

	WorldTransformUpdate(worldTransform_);
}

void Enemy::FireSpreadShot(std::list<FunnelBullet*>& bullets) {
	// 3方向（扇状）に弾を発射
	// プレイヤーの方向を向く処理を入れるとさらに強いが、まずは「手前」に撃つ
	Vector3 baseVelocity = {0.0f, 0.0f, -0.5f}; // 奥から手前へ
	// 敵はZ奥(-方向)にいるので、手前(+方向)へ撃つならZはプラスだが、カメラ設定による
	// ※既存コードを見る限り PlayerBulletは (0,0,1) で進んでいるので、敵弾は (0,0,-1) かもしれない
	// ひとまずカメラ方向（手前）に向かってくると仮定して
	//baseVelocity = {0.0f, 0.0f, 0.5f}; // ★逆なら -0.5f に修正してください

	// 3発生成
	for (int i = -1; i <= 1; ++i) {
		FunnelBullet* newBullet = new FunnelBullet();

		// 速度ベクトルを少し回転させる（拡散ショット）
		Vector3 vel = baseVelocity;
		vel.x += i * 0.2f; // X成分をずらす

		// 初期化（敵の位置から発射）
		newBullet->Initialize(bulletModel_, worldTransform_.translation_, vel);

		// リストに追加
		bullets.push_back(newBullet);
	}
}

// Drawなどは変更なし
void Enemy::Draw() {
	if (isDead_ || !model_ || !camera_)
		return;
	if (flashTimer_ > 0 && (flashTimer_ % 2) == 0) {
		model_->Draw(worldTransform_, *camera_);
	} else if (flashTimer_ <= 0) {
		model_->Draw(worldTransform_, *camera_);
	}
}
void Enemy::OnHit(int damage) {

	if (isDead_) {
		return;
	}

	hp_ -= damage;
	if (hp_ <= 0) {
		isDead_ = true;
	}

	// フラッシュ開始
	flashTimer_ = kFlashDurationFrame;
}

AABB Enemy::GetAABB() const {

	// 適当に箱を作る
	Vector3 halfSize = {2.0f, 2.0f, 2.0f};

	AABB box{};
	const Vector3& pos = worldTransform_.translation_;

	box.min = {pos.x - halfSize.x, pos.y - halfSize.y, pos.z - halfSize.z};
	box.max = {pos.x + halfSize.x, pos.y + halfSize.y, pos.z + halfSize.z};


	return box;
}
