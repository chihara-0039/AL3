#include "GameOverScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

namespace {
bool IsSpacePressed() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void GameOverScene::Initialize() {
	// ゲームオーバー演出の初期化など
}

void GameOverScene::Update() {
	// スペースでタイトルに戻る
	if (IsSpacePressed()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::TITLE);
	}
}

void GameOverScene::Draw() {
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//dxCommon->PreDraw();

	// 「GAME OVER」表示など

	//dxCommon->PostDraw();
}

void GameOverScene::Finalize() {
	// リソース解放
}
