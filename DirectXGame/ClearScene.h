#pragma once
#include "SceneManager.h"
#include "Skydome.h"
#include "Math.h"
#include <Windows.h>

class ClearScene : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw3D() override;
	void Draw2D() override;
	void Finalize() override;

private:
	KamataEngine::Camera camera_;
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* clearModel_ = nullptr;
	uint32_t clearTex_ = 0;
	KamataEngine::Sprite* clearSprite_ = nullptr;
	KamataEngine::WorldTransform clearWT_;
};
