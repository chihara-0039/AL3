#include "Player.h"
#include "Math.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {

	assert(model);
	// モデル
	model_ = model;

	// テクスチャハンドル
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 1.0f;
	worldTransform_.translation_.y = 1.0f;

	camera_ = camera;
}

void Player ::Update() {

	// アフィン変換～DirectXに転送
	WorldTransformUpdate(worldTransform_);
}

void Player ::Draw() {

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}