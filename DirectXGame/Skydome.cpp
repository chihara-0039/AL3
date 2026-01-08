#include "Skydome.h"
#include "Math.h"

#include <Windows.h>

using namespace KamataEngine;

Skydome::~Skydome() { Finalize(); }

void Skydome::Initialize(Camera* camera) {

	camera_ = camera;

	// モデル読み込み（Resources/<name>/<name>.obj を想定）
	delete model_;
	model_ = Model::CreateFromOBJ("skydome");
	OutputDebugStringA("Skydome: FORCE Skydome model.\n");


	worldTransform_.Initialize();

	// ひとまずデカくする（見える確認用）
	SetRadius(80.0f);

	// 最初の位置はカメラの位置にしておく
	if (camera_) {
		worldTransform_.translation_ = camera_->translation_;
		WorldTransformUpdate(worldTransform_);
	}

	// Skydome::Initialize の最後あたりで一回
	worldTransform_.translation_ = {0.0f, 0.0f, 20.0f};
	WorldTransformUpdate(worldTransform_);
}

void Skydome::Update() {
	if (!camera_) {
		return;
	}

	// 常にカメラ位置に固定（天球の“中心”をカメラに合わせる）
	worldTransform_.translation_ = camera_->translation_;

	// ゆっくり回転：横に流れる感じなら Y 回転が基本
	// 0.1度/秒 → 0.1 * π/180 / 60（60fps想定）
	const float kYawPerFrame = (0.1f * 3.14159265f / 180.0f) / 60.0f;
	worldTransform_.rotation_.y += kYawPerFrame;

	// 回転値がでかくなりすぎないようにする（なくても動く）
	if (worldTransform_.rotation_.y > 6.2831853f) {
		worldTransform_.rotation_.y -= 6.2831853f; // 2π
	}

	WorldTransformUpdate(worldTransform_);
}


void Skydome::Draw() {

	if (!model_ || !camera_) {
		return;
	}

	// 3D描画中（Model::PreDraw〜PostDraw の間）で呼ばれる前提
	model_->Draw(worldTransform_, *camera_);
}

void Skydome::Finalize() {

	delete model_;
	model_ = nullptr;
	camera_ = nullptr;
}

void Skydome::SetRadius(float radius) {
	radius_ = radius;

	constexpr float kModelBaseRadius = 500.0f; // SkyDome.obj の元半径
	const float s = radius_ / kModelBaseRadius;

	// 内側表示したいならどれか1軸を反転
	worldTransform_.scale_ = {s, s, s}; // 反転が不要なら {s, s, s}
	WorldTransformUpdate(worldTransform_);
}
