#include "Funnel.h"
#include <cmath>

void Funnel::Initialize(Model* model, Camera* camera, const Vector3& center, float radius, float startAngle) {

	model_ = model;
	camera_ = camera;

	radius_ = radius;
	angle_ = startAngle;

	worldTransform_.Initialize();

	// 少しだけ高さを付ける（必要なければ 0.0f のままでOK）
	heightOffset_ = 0.0f;

	Update(center); // 初回位置を反映
}

void Funnel::Update(const Vector3& center) {

	angle_ += angularSpeed_;

	// XZ 平面で周回させる
	worldTransform_.translation_.x = center.x + std::cos(angle_) * radius_;
	worldTransform_.translation_.z = center.z + std::sin(angle_) * radius_;
	worldTransform_.translation_.y = center.y + heightOffset_;

	// 必要なら回転なども設定する
	// worldTransform_.rotation_.y = angle_;

	WorldTransformUpdate(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Funnel::Draw() {
	if (model_ && camera_) {
		model_->Draw(worldTransform_, *camera_);
	}
}
