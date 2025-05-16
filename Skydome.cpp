#include "Skydome.h"

using namespace KamataEngine;

void Skydome::Initialize(Model* model, Camera* camera) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数の内容を記録
	camera_ = camera;
}

void Skydome::Update() {

	//アフィン変換行列
	worldTransform_.matWorld_ = math_.MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix(); // ここ最後
}

void Skydome::Draw() {

	model_->Draw(worldTransform_, *camera_);
}

