#include "FunnelBullet.h"
#include <cassert>

void FunnelBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	assert(model);
	model_ = model;

	// テクスチャは適当なものを流用
	static uint32_t sTex = TextureManager::Load("FunnelBullet.png");
	textureHandle_ = sTex;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	WorldTransformUpdate(worldTransform_);
	worldTransform_.TransferMatrix();
}

void FunnelBullet::Update() {

	// 位置更新
	worldTransform_.translation_ += velocity_;

	WorldTransformUpdate(worldTransform_);
	worldTransform_.TransferMatrix();

	// 寿命
	if (--lifeTimer_ <= 0) {
		isDead_ = true;
	}
}

void FunnelBullet::Draw(const Camera& camera) {
	if (!isDead_ && model_) {
		model_->Draw(worldTransform_, camera, textureHandle_);
	}
}
