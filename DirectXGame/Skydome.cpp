#include "Skydome.h"
#include "Math.h" // WorldTransformUpdate を使っているなら

using namespace KamataEngine;

void Skydome::Initialize(Camera* camera) {

	camera_ = camera;

	// モデル読み込み（SkyDome.obj / sky_sphere.png）
	model_ = Model::CreateFromOBJ("SkyDome");

	worldTransform_.Initialize();
	worldTransform_.scale_ = {radius_, radius_, radius_};

	// 最初の位置はカメラの位置にしておく
	if (camera_) {
		worldTransform_.translation_ = camera_->translation_;
	}

	WorldTransformUpdate(worldTransform_);
}

void Skydome::Update() {

	if (!camera_) {
		return;
	}

	// カメラ位置に追従させる
	worldTransform_.translation_ = camera_->translation_;

	WorldTransformUpdate(worldTransform_);
}

void Skydome::Draw() {

	if (!model_ || !camera_) {
		return;
	}

	// 3D描画中（Model::PreDraw〜PostDraw の間）で呼ばれる前提
	model_->Draw(worldTransform_, *camera_);
}

void Skydome::SetRadius(float radius) {

	radius_ = radius;
	worldTransform_.scale_ = {radius_, radius_, radius_};
	WorldTransformUpdate(worldTransform_);
}
