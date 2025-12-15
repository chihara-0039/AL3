#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();

	// 位置とサイズは調整してOK
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};

	WorldTransformUpdate(worldTransform_);
}

void Enemy::Update() {

	if (isDead_) {
		return;
	}

	// 被弾フラッシュタイマー更新
	if (flashTimer_ > 0) {
		--flashTimer_;
	}

	// 必要ならここで移動処理など
	// worldTransform_.translation_.z -= 0.0f;

	WorldTransformUpdate(worldTransform_);
}


void Enemy::Draw() {

	 // ★ 死亡済み or モデル/カメラが無いなら描画しない
	if (isDead_ || model_ == nullptr || camera_ == nullptr) {
		return;
	}

	// ★ フラッシュ中は「描画しない」＝点滅して見える
	if (flashTimer_ > 0) {
		// 何もしない → 一瞬消える
		// flashTimer_ が残っている間は点滅させる
		// 偶数フレームのときだけ描画する例
		if ((flashTimer_ % 2) == 0) {
			model_->Draw(worldTransform_, *camera_);
		}
		return;
	}

	// 行列＋色を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
	
	//通常状態
	model_->Draw(worldTransform_, *camera_);
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

	box.min = {pos.x - halfSize_.x, pos.y - halfSize_.y, pos.z - halfSize_.z};
	box.max = {pos.x + halfSize_.x, pos.y + halfSize_.y, pos.z + halfSize_.z};

	return box;
}
