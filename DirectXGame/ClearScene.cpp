// ClearScene.cpp
#include "ClearScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

namespace {
bool IsSpacePressed() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void ClearScene::Initialize() {
	// クリア演出の初期化など
}

void ClearScene::Update() {
	// スペースでタイトルに戻る
	if (IsSpacePressed()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::TITLE);
	}
}

void ClearScene::Draw() {
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//dxCommon->PreDraw();

	// 「GAME CLEAR」表示など

	//dxCommon->PostDraw();
}

void ClearScene::Finalize() {
	// リソース解放
}
