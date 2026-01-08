// ClearScene.cpp
#include "ClearScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

namespace {
bool IsSpacePressed() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void ClearScene::Initialize() {
	// クリア演出の初期化など
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.UpdateMatrix();

	skydome_ = new Skydome();
	skydome_->Initialize(&camera_);
	skydome_->SetRadius(200.0f);
}

void ClearScene::Update() {
	camera_.UpdateMatrix();
	if (skydome_) {
		skydome_->Update();
	}

	if (IsSpacePressed()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::TITLE);
	}
}

void ClearScene::Draw3D() {
	if (skydome_)
		skydome_->Draw(); // ★Model::PreDraw〜PostDrawの間で呼ばれる前提
}
void ClearScene::Finalize() {
	// リソース解放
	delete skydome_;
	skydome_ = nullptr;
}

void ClearScene::Draw2D() {
	// 2D表示（クリア画像など）を入れるならここ
}
