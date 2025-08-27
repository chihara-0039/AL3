#include "Enemy.h"
#include "GameScene.h"
#include "MapChipField.h"
#include <cassert>
#include <cmath>
#include <numbers>

// 初期化
void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // 左向き
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer = 0.0f;
}

// 壁衝突チェック
void Enemy::CheckMapCollision() {
	if (!mapChipField_)
		return; 

	// 右側
	MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition({worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y, worldTransform_.translation_.z});
	if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
		velocity_.x = -std::abs(velocity_.x);
		worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // 左向き
		direction_ = Direction::kLeft;
		return;
	}

	// 左側
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({worldTransform_.translation_.x - kWidth / 2.0f, worldTransform_.translation_.y, worldTransform_.translation_.z});
	if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
		velocity_.x = std::abs(velocity_.x);
		worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 右向き
		direction_ = Direction::kRight;
		return;
	}
}

// 更新
void Enemy::Update() {
	// 振る舞い変更リクエスト
	if (behaviorRequest_ != Behavior::kUnknown) {
		behavior_ = behaviorRequest_;
		switch (behavior_) {
		case Behavior::kDefeated:
			counter_ = 0;
			break;
		default:
			break;
		}
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Behavior::kWalk: {
		// 壁判定して方向反転
		CheckMapCollision();

		// 移動
		worldTransform_.translation_ += velocity_;

		// 歩行アニメーション
		walkTimer += 1.0f / 60.0f;
		worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

		WorldTransformUpdate(worldTransform_);
		break;
	}

	case Behavior::kDefeated: {
		counter_ += 1.0f / 60.0f;
		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);
		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isDead_ = true;
		}
		break;
	}
	}
}

// 描画
void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

// AABB取得
AABB Enemy::GetAABB() {
	Vector3 pos = GetWorldPosition();
	return {
	    {pos.x - kWidth / 2.0f, pos.y - kHeight / 2.0f, pos.z - kWidth / 2.0f},
        {pos.x + kWidth / 2.0f, pos.y + kHeight / 2.0f, pos.z + kWidth / 2.0f}
    };
}

// ワールド座標取得
Vector3 Enemy::GetWorldPosition() { return {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]}; }

// プレイヤーとの衝突
void Enemy::OnCollision(const Player* player) {
	if (behavior_ == Behavior::kDefeated)
		return;
	if (player->IsAttack()) {
		if (gameScene_) {
			Vector3 pos = player->GetWorldPosition();
			Vector3 effectPos = (GetWorldPosition() + pos) * 0.5f;
			gameScene_->CreateEffect(effectPos);
		}
		behaviorRequest_ = Behavior::kDefeated;
		isCollisionDisabled_ = true;
	}
}
