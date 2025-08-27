#include "GameOverScene.h"
#include "Math.h"
#include <numbers>

GameOverScene::~GameOverScene() {
	delete modelPlayer_;
	delete modelGameOver_;
	delete fade_; // ← 追加：後始末
	delete skydome_;
	delete modelSkydome_;
}

void GameOverScene::Initialize() {
	// モデル読み込み
	modelGameOver_ = Model::CreateFromOBJ("GameOverFont", true);
	modelPlayer_ = Model::CreateFromOBJ("Player", true);

	// カメラ初期化
	camera_.Initialize();

	// タイトル文字のTransform
	const float kPlayerTitle = 2.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kPlayerTitle, kPlayerTitle, kPlayerTitle};

	// プレイヤー人形のTransform
	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -10.0f;

	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	// --- 追加：登場時は黒→フェードイン ---
	fade_ = new Fade();
	fade_->Initialize();
	// GameScene 側で真っ黒までフェードアウト済み → ここで黒から1.0秒かけて明るく
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameOverScene::Update() {
	// （演出）GAME OVER 文字を上下にゆらす
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeFontMove);
	float angle = counter_ / kTimeFontMove * 2.0f * std::numbers::pi_v<float>;
	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	// 入力：スペースでタイトルへ戻る
	// すぐ抜けたい場合は下の2行だけでOK:
	//    if (Input::GetInstance()->TriggerKey(DIK_SPACE)) finished_ = true;
	// ただし見栄えのため、フェードアウトしてから抜ける仕様にしてます。
	if (!requestExit_ && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			requestExit_ = true;
			fade_->Start(Fade::Status::FadeOut, 0.8f); // 0.8秒で暗転→抜ける
	}

	// ★天球の更新
	if (skydome_) {
		skydome_->Update();
	}

	// カメラ/行列更新
	camera_.UpdateMatrix();
	WorldTransformUpdate(worldTransformTitle_);
	WorldTransformUpdate(worldTransformPlayer_);

	// フェード更新
	if (fade_) {
		fade_->Update();
		// 退出要求中にフェードが終わったらシーン終了
		if (requestExit_ && fade_->IsFinished()) {
			finished_ = true;
		}
	}
}

void GameOverScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	if (skydome_) {
		skydome_->Draw();
	}
	modelGameOver_->Draw(worldTransformTitle_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	Model::PostDraw();

	// --- 追加：最後にフェードを重ね描き ---
	if (fade_) {
		fade_->Draw();
	}
}
