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

	if (isDead_) {
		return;
	}


	// ★Model に色を設定するAPIがある場合はここで赤フラッシュ
	/*
	if (flashTimer_ > 0) {
	    // 被弾中は赤
	    model_->SetColor({1.0f, 0.0f, 0.0f, 1.0f});
	} else {
	    // 通常時は白
	    model_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}
	*/

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

	AABB box{};
	const Vector3& pos = worldTransform_.translation_;

	box.min = {pos.x - halfSize_.x, pos.y - halfSize_.y, pos.z - halfSize_.z};
	box.max = {pos.x + halfSize_.x, pos.y + halfSize_.y, pos.z + halfSize_.z};

	return box;
}
