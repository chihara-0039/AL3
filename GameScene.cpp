#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	// ここにデータの解放処理を書く
	// 3Dモデルデータの解放
	delete model_;
}

void GameScene::Initialize() {

	//メンバ変数への初期化処理を書く
	
	//  3Dモデルデータの生成
	textureHandle_ = TextureManager::Load("uvChecker.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//ここにインゲームの初期化処理を書く
}

void GameScene::Update() {
	//ここにインゲームの更新処理を書く
}

void GameScene::Draw() {
	// 描画処理
	model_->Draw(viewProjection_.matViewProj, textureHandle_);
}