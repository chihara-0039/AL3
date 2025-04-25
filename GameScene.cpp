#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	//メンバ変数への初期化処理を書く
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvCheacker.png");

	//3Dモデル生成
	model_ = Model::Create();


	//ここにインゲームの初期化処理を書く
}

void GameScene::Update() {
	//ここにインゲームの更新処理を書く
}

void GameScene::Draw() {
	// 描画処理
	
}