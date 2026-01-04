#include "Player.h"
#include "Math.h"
#include <cassert>
#include <cmath>

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();

	// 初期位置
	worldTransform_.translation_ = position;

	// サイズ調整
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// 移動速度
	const float kCharacterSpeed = 0.01f;

	const float kFriction = 0.9f;
	const float kMaxSpeed = 3.0f;

	if (input_->PushKey(DIK_W)) { // 前へ
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_S)) { // 後ろへ
		move.y -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_D)) { // 右へ
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_A)) { // 左へ
		move.x -= kCharacterSpeed;
	}

	velocity_ += move;

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	// 速度の制限
	if (Length(velocity_) > kMaxSpeed) {
		velocity_ = Normalized(velocity_) * kMaxSpeed;
	}

	// 摩擦で減速
	velocity_ *= kFriction;

	// 実際に移動
	worldTransform_.translation_ += velocity_;

	// 移動制限座標
	// X軸（左右）
	const float kMoveLimitX = 6.0f;

	// Y軸（上下）
	const float kMoveLimitY = 3.0f;

	// 移動制限
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 無敵タイマー
	if (invincibleTimer_ > 0) {
		--invincibleTimer_;
	}

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 行列更新
	WorldTransformUpdate(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	
	// モデル or カメラが無ければ何もしない
	if (model_ == nullptr || camera_ == nullptr) {
		return;
	}

	// 死亡済みなら描画しない
	if (hp_ <= 0) {
		return;
	}


	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}

	 // 無敵中は点滅（フレームカウントでON/OFFを切り替える）
	if (invincibleTimer_ > 0) {
		// 2フレームごとに ON/OFF
		if ((invincibleTimer_ / 2) % 2 == 0) {
			// 描画する
			model_->Draw(worldTransform_, *camera_);
		} else {
			// このフレームは描画しない
			return;
		}
	} else {
		// 通常
		model_->Draw(worldTransform_, *camera_);
	}
	// プレイヤーモデル描画
	model_->Draw(worldTransform_, *camera_);
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 通常弾のダメージは1としておく
		const int32_t kBulletDamage = 1;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity, kBulletDamage);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnHit(int famage) {
	//すでに死んでいる or 無敵中なら無視
	if (hp_ <= 0 || invincibleTimer_ > 0) {
		return;
	}

	hp_ -= famage;
	if (hp_ < 0) {
		hp_ = 0; 
	}

	// 無敵タイマーセット
	invincibleTimer_ = kInvincibleDuration;
}

// 狙い点へ撃つAPI（マウス照準用）
void Player::FireToward(const Vector3& targetWorld) {

	// 弾の速度
	const float kBulletSpeed = 1.5f;

	// 自機から狙い点へのベクトル計算
	Vector3 direction = targetWorld - worldTransform_.translation_;

	// 正規化して速度ベクトルを作成
	direction = Normalized(direction);

	// 速度ベクトルに弾速を掛ける
	Vector3 velocity = direction * kBulletSpeed;

	// マウス弾もダメージ1（後で変更したければここを変える）
	const int32_t kBulletDamage = 1;

	// 弾を生成し、初期化
	PlayerBullet* newBullet = new PlayerBullet();

	// 弾の初期位置を自機の位置に設定
	newBullet->Initialize(model_, worldTransform_.translation_, velocity, kBulletDamage);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

Player::~Player() {

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}
