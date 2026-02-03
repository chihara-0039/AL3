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

	// クリア画像の読み込みobj
	clearModel_ = Model::CreateFromOBJ("clearFont");

	clearWT_.Initialize();
	clearWT_.translation_ = {0.0f, 0.0f, 0.0f};
	clearWT_.scale_ = {1.0f, 1.0f, 1.0f};
	clearWT_.rotation_ = {0.0f, 0.0f, 0.0f}; // ★タイトルは回さない
	WorldTransformUpdate(clearWT_);
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
	
	skydome_->Draw(); // ★Model::PreDraw〜PostDrawの間で呼ばれる前提
	clearModel_->Draw(clearWT_, camera_);
}

void ClearScene::Finalize() {
	// リソース解放
	delete skydome_;
	skydome_ = nullptr;

	delete clearModel_;
	clearModel_ = nullptr;
}

void ClearScene::Draw2D() {
	// 2D描画（スプライトなど）が必要なければ空っぽでOK
}
