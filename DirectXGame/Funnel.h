#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

/// 画面奥に浮いているファンネル本体
class Funnel {
public:
	/// 初期化
	/// center … 回転の中心（今回は「画面奥の中心」想定）
	/// radius … 中心からの距離
	/// startAngle … 初期角度（ラジアン）
	void Initialize(Model* model, Camera* camera, const Vector3& center, float radius, float startAngle);

	/// 更新（center の周囲を公転させる）
	void Update(const Vector3& center);

	/// 描画
	void Draw();

	/// ワールド座標（ビームの発射位置用）
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;

	float radius_ = 5.0f;                  // 中心からの距離
	float angle_ = 0.0f;                   // 現在の角度（ラジアン）
	float angularSpeed_ = ToRadians(1.5f); // 1.5度/フレームくらいで回る
	float heightOffset_ = 0.0f;            // 中心より少し上下にずらしたいとき用
};
