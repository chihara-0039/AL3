#include "HpHud.h"
#include "Enemy.h"
#include "Player.h"

#include <Windows.h>
#include <cstdio>
#include <imgui.h>

void HpHud::Draw(const Player* player, const Enemy* enemy) {

	// ===== ImGuiが回ってない（コンテキスト無し）なら描けない =====
	if (ImGui::GetCurrentContext() == nullptr) {
		static bool warned = false;
		if (!warned && config_.warnIfNoImGui) {
			warned = true;
			OutputDebugStringA("[HpHud] ImGui context is null. "
			                   "HP HUD will not be visible unless ImGui::NewFrame/Render is running in the engine loop.\n");
		}
		return;
	}

	if (!player || !enemy) {
		return;
	}

	// 値取得
	const int pHP = player->GetHP();
	const int pMax = Player::kMaxHP;

	const int eHP = enemy->GetHP();
	const int eMax = Enemy::kMaxHP;

	// 画面情報
	ImGuiViewport* vp = ImGui::GetMainViewport();
	const ImVec2 pos = vp->WorkPos;
	const ImVec2 size = vp->WorkSize;

	ImDrawList* dl = ImGui::GetForegroundDrawList();

	// ===== 敵HP：上中央 =====
	{
		char text[64];
		sprintf_s(text, "ENEMY HP  %d / %d", eHP, eMax);

		ImVec2 ts = ImGui::CalcTextSize(text);
		ImVec2 tp(pos.x + size.x * 0.5f - ts.x * 0.5f, pos.y + config_.enemyTopPadding);
		dl->AddText(tp, IM_COL32(255, 255, 255, 255), text);

		const float barW = config_.enemyBarW;
		const float barH = config_.enemyBarH;

		ImVec2 barMin(pos.x + size.x * 0.5f - barW * 0.5f, tp.y + ts.y + 6.0f);
		ImVec2 barMax(barMin.x + barW, barMin.y + barH);

		float t = (eMax > 0) ? (float)eHP / (float)eMax : 0.0f;
		if (t < 0.0f)
			t = 0.0f;
		if (t > 1.0f)
			t = 1.0f;

		// 背景＆枠
		dl->AddRectFilled(barMin, barMax, IM_COL32(0, 0, 0, 160));
		dl->AddRect(barMin, barMax, IM_COL32(255, 255, 255, 255));

		// 充填
		ImVec2 fillMax(barMin.x + barW * t, barMax.y);
		dl->AddRectFilled(barMin, fillMax, IM_COL32(255, 80, 80, 220));
	}

	// ===== プレイヤーHP：左下 =====
	{
		char text[64];
		sprintf_s(text, "PLAYER HP  %d / %d", pHP, pMax);

		ImVec2 ts = ImGui::CalcTextSize(text);

		ImVec2 tp(pos.x + config_.playerLeftPadding, pos.y + size.y - ts.y - config_.playerBottomPadding - 20.0f);
		dl->AddText(tp, IM_COL32(255, 255, 255, 255), text);

		const float barW = config_.playerBarW;
		const float barH = config_.playerBarH;

		ImVec2 barMin(pos.x + config_.playerLeftPadding, tp.y + ts.y + 6.0f);
		ImVec2 barMax(barMin.x + barW, barMin.y + barH);

		float t = (pMax > 0) ? (float)pHP / (float)pMax : 0.0f;
		if (t < 0.0f)
			t = 0.0f;
		if (t > 1.0f)
			t = 1.0f;

		// 背景＆枠
		dl->AddRectFilled(barMin, barMax, IM_COL32(0, 0, 0, 160));
		dl->AddRect(barMin, barMax, IM_COL32(255, 255, 255, 255));

		// 充填
		ImVec2 fillMax(barMin.x + barW * t, barMax.y);
		dl->AddRectFilled(barMin, fillMax, IM_COL32(120, 220, 120, 220));
	}
}
