#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SceneManager.h"
#include "Skydome.h"

class TitleScene : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw3D() override; // ★DrawはIScene側（Draw3D/Draw2D呼び出し）に任せる
	void Finalize() override;

private:
	KamataEngine::Camera camera_;
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::WorldTransform titleWT_;

	//BGM用ハンドル
	uint32_t bgmHandle_ = 0;   // 音データ読み込み用
	uint32_t voiceHandle_ = 0; // 再生管理用（止めるため）
};
