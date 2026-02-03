#pragma once
#include <cstdint>

class Player;
class Enemy;

struct HpHudConfig {
	// 敵HP（上）
	float enemyBarW = 360.0f;
	float enemyBarH = 16.0f;
	float enemyTopPadding = 10.0f;

	// プレイヤーHP（左下）
	float playerBarW = 240.0f;
	float playerBarH = 14.0f;
	float playerLeftPadding = 10.0f;
	float playerBottomPadding = 10.0f;

	// デバッグ：ImGuiが無い時にログを出すか
	bool warnIfNoImGui = true;
};

class HpHud {
public:
	HpHud() = default;

	// HP HUD 描画（ImGuiが回ってないなら表示されないが落ちない）
	void Draw(const Player* player, const Enemy* enemy);

	HpHudConfig& GetConfig() { return config_; }

private:
	HpHudConfig config_;
};
