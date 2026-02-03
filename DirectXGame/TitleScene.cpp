#include "TitleScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

namespace {
bool IsSpaceDown() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void TitleScene::Initialize() {
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.UpdateMatrix();

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(&camera_);
	skydome_->SetRadius(200.0f);

	// タイトルモデル（Resources/Title/Title.obj を想定）
	titleModel_ = Model::CreateFromOBJ("Title");

	titleWT_.Initialize();
	titleWT_.translation_ = {0.0f, 0.0f, 0.0f};
	titleWT_.scale_ = {1.0f, 1.0f, 1.0f};
	titleWT_.rotation_ = {0.0f, 0.0f, 0.0f}; // ★タイトルは回さない
	WorldTransformUpdate(titleWT_);
}

void TitleScene::Update() {
	camera_.UpdateMatrix();

	if (skydome_) {
		skydome_->Update();
	}

	// ★回転処理は削除（タイトルは回らない）
	// titleWT_.rotation_.y += 0.01f;
	// WorldTransformUpdate(titleWT_);

	if (IsSpaceDown()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::GAME);
	}

	titleWT_.rotation_ = {0, 0, 0};
	WorldTransformUpdate(titleWT_);
}

void TitleScene::Draw3D() {
	// ※PreDraw/PostDraw/Model::PreDraw/PostDraw は SceneManager 側でまとめて実行する
	
	skydome_->Draw();
	

	titleModel_->Draw(titleWT_, camera_);
}

void TitleScene::Finalize() {
	delete skydome_;
	skydome_ = nullptr;

	delete titleModel_;
	titleModel_ = nullptr;
}
