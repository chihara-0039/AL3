#include "HpHud.h"
#include "Enemy.h"
#include "Player.h"

using namespace KamataEngine;

void HpHud::Initialize() {
	// 白い画像を読み込む
	textureHandle_ = TextureManager::Load("white1x1.png");

	// === プレイヤー用バー設定 (左下) ===
	Vector2 playerPos = {50.0f, 600.0f}; // 画面左下あたり
	Vector2 playerSize = {kPlayerBarWidth_, 30.0f};

	// 背景（黒）
	playerBarBG_ = Sprite::Create(textureHandle_, playerPos);
	playerBarBG_->SetSize(playerSize);
	playerBarBG_->SetColor({0.0f, 0.0f, 0.0f, 1.0f}); // 黒

	// 前景（緑）
	playerBarFG_ = Sprite::Create(textureHandle_, playerPos);
	playerBarFG_->SetSize(playerSize);
	playerBarFG_->SetColor({0.0f, 1.0f, 0.0f, 1.0f}); // 緑

	// === 敵用バー設定 (上中央) ===
	// 画面幅が1280と仮定して中央配置
	Vector2 enemyPos = {(1280.0f - kEnemyBarWidth_) / 2.0f, 20.0f};
	Vector2 enemySize = {kEnemyBarWidth_, 20.0f};

	// 背景（黒・少し透明）
	enemyBarBG_ = Sprite::Create(textureHandle_, enemyPos);
	enemyBarBG_->SetSize(enemySize);
	enemyBarBG_->SetColor({0.0f, 0.0f, 0.0f, 0.5f});

	// 前景（赤）
	enemyBarFG_ = Sprite::Create(textureHandle_, enemyPos);
	enemyBarFG_->SetSize(enemySize);
	enemyBarFG_->SetColor({1.0f, 0.0f, 0.0f, 1.0f});
}

void HpHud::Draw(const Player* player, const Enemy* enemy) {
	// スプライト描画前処理（GameSceneのDraw2Dで呼ばれている前提なら不要だが念のため）
	// ※親側でPreDrawしているならここは無くても動きます

	// --- プレイヤーHP更新 ---
	if (player && playerBarFG_) {
		float ratio = 0.0f;
		float maxHp = (float)Player::kMaxHP;
		if (maxHp > 0) {
			ratio = (float)player->GetHP() / maxHp;
		}
		// 0～1の範囲に収める
		if (ratio < 0.0f)
			ratio = 0.0f;
		if (ratio > 1.0f)
			ratio = 1.0f;

		// 幅を変更
		playerBarFG_->SetSize({kPlayerBarWidth_ * ratio, 30.0f});

		// 描画
		playerBarBG_->Draw();
		playerBarFG_->Draw();
	}

	// --- 敵HP更新 ---
	if (enemy && enemyBarFG_) {
		float ratio = 0.0f;
		float maxHp = (float)Enemy::kMaxHP;
		if (maxHp > 0) {
			ratio = (float)enemy->GetHP() / maxHp;
		}
		if (ratio < 0.0f)
			ratio = 0.0f;
		if (ratio > 1.0f)
			ratio = 1.0f;

		// 幅を変更
		enemyBarFG_->SetSize({kEnemyBarWidth_ * ratio, 20.0f});

		// 描画
		enemyBarBG_->Draw();
		enemyBarFG_->Draw();
	}
}

void HpHud::Finalize() {
	delete playerBarBG_;
	delete playerBarFG_;
	delete enemyBarBG_;
	delete enemyBarFG_;
}