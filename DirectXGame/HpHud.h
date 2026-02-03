#pragma once
#include "KamataEngine.h"

// 前方宣言
class Player;
class Enemy;

class HpHud {
public:
	// 初期化（画像を読み込み、スプライトを作る）
	void Initialize();

	// 描画（HPを受け取ってバーの長さを変える）
	void Draw(const Player* player, const Enemy* enemy);

	// 終了処理（スプライトのメモリ解放）
	void Finalize();

private:
	// 共通のテクスチャハンドル
	uint32_t textureHandle_ = 0;

	// --- プレイヤー用 ---
	KamataEngine::Sprite* playerBarBG_ = nullptr; // 背景（黒）
	KamataEngine::Sprite* playerBarFG_ = nullptr; // 前景（緑）
	const float kPlayerBarWidth_ = 300.0f;        // プレイヤーバーの最大幅

	// --- 敵用 ---
	KamataEngine::Sprite* enemyBarBG_ = nullptr;
	KamataEngine::Sprite* enemyBarFG_ = nullptr;
	const float kEnemyBarWidth_ = 400.0f; // 敵バーの最大幅
};