#include "GameOverScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

namespace {
bool IsSpacePressed() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void GameOverScene::Initialize() {
	// ゲームオーバー演出の初期化など
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.UpdateMatrix();

	skydome_ = new Skydome();
	skydome_->Initialize(&camera_);
	skydome_->SetRadius(200.0f);

	// ゲームオーバー画像の読み込みobj
	// ※ Resources/GameOverFont/GameOverFont.obj があるか確認してください
	gameOverModel_ = Model::CreateFromOBJ("GameOverFont");

	// ★初期化処理を追加（TitleScene同様に行う）
	gameOverWT_.Initialize();
	gameOverWT_.translation_ = {0.0f, 0.0f, 0.0f};
	gameOverWT_.scale_ = {1.0f, 1.0f, 1.0f};
	gameOverWT_.rotation_ = {0.0f, 0.0f, 0.0f};
	
}

void GameOverScene::Update() {
	camera_.UpdateMatrix();
	if (skydome_) {
		skydome_->Update();
	}

	if (IsSpacePressed()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::TITLE);
	}
}

void GameOverScene::Draw3D() {
	if (skydome_) {
		skydome_->Draw();
	}
	gameOverModel_->Draw(gameOverWT_, camera_);
}

void GameOverScene::Finalize() {
	delete skydome_;
	skydome_ = nullptr;

	delete gameOverModel_;
	gameOverModel_ = nullptr;
}

void GameOverScene::Draw2D() {}