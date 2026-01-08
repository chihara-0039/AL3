#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, int32_t damage) {

	// NULLポインタチェック
	

	// モデル
	model_ = model;

	// テクスチャ読み込み
	static uint32_t sTex = TextureManager::Load("PlayerBeam.png");
	textureHandle_ = sTex;


	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバー変数に代入
	velocity_ = velocity;

	// ダメージ設定
	damage_ = damage;

	// 初期フレームから正しい位置で描画されるようにワールドトランスフォームを更新
	WorldTransformUpdate(worldTransform_);
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Update() {

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	// ワールドトランスフォームの更新
	WorldTransformUpdate(worldTransform_);
	worldTransform_.TransferMatrix();

	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const Camera& camera) { model_->Draw(worldTransform_, camera, textureHandle_); }
