#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	//////////////////
	// 関数AL3_02_02//
	///////////////////
	GameScene();
	~GameScene();
	// 平行移動行列
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// 拡大縮小行列
	Matrix4x4 Matrix4x4MakeScaleMatrix(const Vector3& scale);

	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// 各軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);
	Matrix4x4 MakeRotateYMatrix(float radian);
	Matrix4x4 MakeRotateZMatrix(float radian);

	// 行列の積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	// ビュー行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

private:
	// テクスチャハンドル
	uint32_t textureHandle = 0;

	// 3Dモデルデータ
	Model* model = nullptr;
	

	// カメラ
	Camera* camera;

	// 自キャラ
	Player* player_ = nullptr;

	// 3Dモデルデータ(block)AL3_02_02
	Model* modelBlock = nullptr;

	//3Dモデルデータ(skydome)
	Model* modelSkydome_ = nullptr;

	// ブロック用のワールドトランスフォームAL3_0202
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks;
	//天球
	Skydome* skydome_ = nullptr;

	// デバックカメラ有効AL3_02_02
	bool isDebugCameraActive = false;
	// デバッグカメラAL3_0202
	DebugCamera* debugCamera = nullptr;
};