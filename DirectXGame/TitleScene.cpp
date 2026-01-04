#include "TitleScene.h"
#include "GameScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

namespace {
bool IsSpacePressed() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void TitleScene::Initialize() {
	// 必要ならタイトル用のスプライトやBGM初期化
}

void TitleScene::Update() {
	// スペース押したらゲーム開始
	if (IsSpacePressed()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::GAME);
	}
}

void TitleScene::Draw() {

	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ここにタイトル表示（ImGuiやSpriteなど）
	// 今は何も描かなくてもOK

	//dxCommon->PostDraw();
}

void TitleScene::Finalize() {
	// タイトル用のリソース解放があればここで
}
