#pragma once
#include "Funnel.h"
#include "FunnelBullet.h"
#include "KamataEngine.h"
#include "Player.h"
#include "Enemy.h"
#include <list>
#include <memory>
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	void Delete();

private:
	// プレイヤー
	Player* player_ = nullptr;
	// プレイヤー用モデル
	Model* player_model_ = nullptr;

	 // 敵（ボス）
	Enemy* enemy_ = nullptr;
	Model* enemy_model_ = nullptr;

	// カメラ
	Camera camera_;

	// ファンネル本体
	std::vector<Funnel*> funnels_;
	Model* funnel_model_ = nullptr;

	// ファンネル弾
	std::list<FunnelBullet*> funnelBullets_;
	Model* funnelBullet_model_ = nullptr;
	int funnelAttackTimer_ = 0;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラの有効フラグ
	bool isDebugCameraActive_ = false;
};
