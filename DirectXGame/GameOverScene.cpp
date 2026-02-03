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
	gameOverModel_ = Model::CreateFromOBJ("GameOverFont");
}

void GameOverScene::Update() {
	// スペースでタイトルに戻る
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

	// ゲームオーバーモデルの描画
	gameOverModel_->Draw(gameOverWT_, camera_);
}

void GameOverScene::Finalize() {
	// リソース解放
	delete skydome_;
	skydome_ = nullptr;

	delete gameOverModel_;
	gameOverModel_ = nullptr;
}

void GameOverScene::Draw2D() {
	// 2D表示（GAME OVER画像など）を入れるならここ
}
