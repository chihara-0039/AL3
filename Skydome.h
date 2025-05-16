#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

/// <summary>
///	天球
/// </summary>
class Skydome {
public:

	/// <summary>
	///	初期化
	/// </summary>
	void Initialize(Model* model, Camera* camera);

	/// <summary>
	///	更新
	/// </summary>
	void Update();

	/// <summary>
	///	描画
	/// </summary>
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	//数学関数
	Math math_;
};
