#pragma once
#include "Enemy.h"
#include "Funnel.h"
#include "FunnelBullet.h"
#include "HpHud.h"
#include "KamataEngine.h"
#include "Player.h"
#include "SceneManager.h"
#include "Skydome.h"

#include <list>
#include <memory>
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene : public IScene {
public:
	void Initialize() override;
	void Update() override;

	// ★これが無いと GameScene.cpp の Draw() 定義で C2509 になる
	void Draw() override;
	void Draw2D() override;
	void Draw3D() override;
	void Finalize() override;

private:
	Player* player_ = nullptr;
	Model* player_model_ = nullptr;
	Model* playerBullet_model_ = nullptr;

	Enemy* enemy_ = nullptr;
	Model* enemy_model_ = nullptr;

	Camera camera_;
	Skydome* skydome_ = nullptr;

	std::vector<Funnel*> funnels_;
	Model* funnel_model_ = nullptr;

	std::list<FunnelBullet*> funnelBullets_;
	Model* funnelBullet_model_ = nullptr;

	int funnelAttackTimer_ = 0;
	uint32_t textureHandle_ = 0u;

	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	enum class BossPattern { FunnelGatling, FunnelBeam };
	BossPattern currentPattern_ = BossPattern::FunnelGatling;
	int patternTimer_ = 0;

	// ★HUD
	HpHud hpHud_;

	// 操作説明画像用
	uint32_t guideTextureHandle_ = 0;
	KamataEngine::Sprite* guideSprite_ = nullptr;

	//BGM用
	uint32_t bgmHandle_ = 0;
	uint32_t voiceHandle_ = 0;
};
