#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Skydome {
public:
	// カメラだけ渡せば中で SkyDome.obj を読む形にする
	void Initialize(Camera* camera);

	// カメラの位置に追従させる
	void Update();

	// 描画（3D描画中に呼ぶだけ）
	void Draw();

	// 明示的に破棄したい時用（Scene::Finalize で呼べる）
	void Finalize();

	// 大きさ調整したくなったとき用（任意）
	void SetRadius(float radius);

	Skydome() = default;
	~Skydome();
	

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;   // SkyDome.obj
	Camera* camera_ = nullptr; // 外から借りるだけ（所有しない）

	float radius_ = 100.0f;
};
