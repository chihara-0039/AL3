#include "Player.h"
#include <cassert>

// 初期化
void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle_, KamataEngine::Camera* camera) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// 引数の内容を記録
	camera_ = camera;

	// テクスチャハンドル
	textureHandle_ = textureHandle;
	worldTransform.Initialize();
}

// 更新
void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform.TransferMatrix(); // ここ最後
}

// 描画
void Player::Draw() {

	model_->Draw(worldTransform, *camera_, textureHandle);

	/////
}

// コンストラクタ
Player::Player() {}

// デストラクタ
Player::~Player() {}