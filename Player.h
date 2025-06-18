#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class MapChipField;

class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	/// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	//入力処理
	void InputMove();

	/// 更新
	void Update();

	/// 描画
	void Draw();

	//外部からポインタをセットするためのsetter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	
	const Vector3& GetVelocity() const { return velocity_; }

private:

	//キャラクターの当たり判定サイズ
	//ブロックよりキャラクターを小さくして1ブロック分の隙を無理なく通れるようにする
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Camera* camera_ = nullptr;
	// 移動量
	Vector3 velocity_ = {};
	// フレームごとの加速度
	static inline const float kAcceleration = 0.01f;
	// 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;
	// 最高速度
	static inline const float kLimitRunSpeed = 0.3f;
	// 顔の向き
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間 <秒>
	static inline const float kTimeTurn = 0.3f;
	// 着地フラグ
	bool onGround_ = true;
	// ジャンプ定数もろもろ
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
};