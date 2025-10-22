#define NOMINMAX
#include "GameScene.h"
#include <Windows.h>
#include <cmath>

using namespace KamataEngine;

static bool KeyDown(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }
bool GameScene::KeyPressed(int vk) {
	static uint8_t latch[256]{};
	bool now = KeyDown(vk), was = latch[vk] != 0;
	latch[vk] = now ? 1 : 0;
	return now && !was;
}

GameScene::~GameScene() {
	delete model_;
	model_ = nullptr;
}

void GameScene::Initialize() {
	// テクスチャ
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// モデル（頂点からの箱）
	model_ = Model::Create();

	// ▼ ViewProjection（＝カメラ）初期化
	viewProjection_.Initialize();
	// 必要なら初期姿勢を少し調整（プロパティ名は教材に合わせて）
	viewProjection_.eye = {0.0f, 2.0f, -6.0f};
	viewProjection_.target = {0.0f, 1.0f, 0.0f};
	// 変更したら行列を更新
	viewProjection_.UpdateMatrix();

	// ▼ WorldTransform（＝オブジェクト）初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = {1.5f, 1.5f, 1.5f};
	worldTransform_->rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_->translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_->TransferMatrix(); // GPUへ反映
}

void GameScene::Update() {
	// F1で原点が見える位置にスナップ（押しっぱなし防止）
	if (KeyPressed(VK_F1)) {
		viewProjection_.eye = {0.0f, 2.0f, -6.0f};
		viewProjection_.target = {0.0f, 1.0f, 0.0f};
	}

	// VPのプロパティを触ったら毎フレ更新
	viewProjection_.UpdateMatrix();

	// 箱を回転（動作確認）
	angleY_ += 0.01f;
	worldTransform_->rotation_.y = angleY_;
	worldTransform_->TransferMatrix(); // 毎フレ呼ぶ！
}

void GameScene::Draw() {
	// DX12 コマンドリストをModelへ（毎フレ必須）
	auto* dx = DirectXCommon::GetInstance();
	auto* cmd = dx->GetCommandList();
	Model::PreDraw(cmd);

	// 教材のシグネチャに合わせて呼ぶ：
	// 多くの環境でこれ（テクスチャあり版）か、テクスチャ無し版
	model_->Draw(*worldTransform_, viewProjection_, textureHandle_);
	// もし上が存在しなければ↓に切替
	// model_->Draw(*worldTransform_, viewProjection_);

	Model::PostDraw();
}
