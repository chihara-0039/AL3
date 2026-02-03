#pragma once
#include "SceneManager.h"
#include "Skydome.h"
#include <Windows.h>

class GameOverScene : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw3D() override;
	void Draw2D() override;
	void Finalize() override;

private:
	KamataEngine::Camera camera_;
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* gameOverModel_ = nullptr;
	KamataEngine::WorldTransform gameOverWT_;
	
};
