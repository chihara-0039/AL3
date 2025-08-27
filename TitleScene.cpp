#include "TitleScene.h"
#include "Math.h"
#include <algorithm>
#include <numbers>

TitleScene::~TitleScene() {
	delete modelPlayer_;
	delete modelTitle_;
	delete fade_;
	delete skydome_;
	delete modelSkydome_;
}

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player"); // 環境に応じて "Player" ならそちらで

	// カメラ初期化
	camera_.Initialize();

	 skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelSkydome_, &camera_);


	// タイトル文字
	const float kTitleScale = 2.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTitleScale, kTitleScale, kTitleScale};

	// プレイヤー
	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>; // 向きはお好みで
	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -10.0f;

	basePlayerY_ = worldTransformPlayer_.translation_.y;
	runTime_ = 0.0f;

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	phase_ = Phase::kFadeIn;
	finished_ = false;
}

void TitleScene::Update() {
	// フェーズ更新
	switch (phase_) {
	case Phase::kFadeIn: {
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
	} break;

	case Phase::kMain: {
		// Space で走り抜け演出へ移行
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::kStartRun;
			runTime_ = 0.0f;

			// 左端から走り始める
			worldTransformPlayer_.translation_.x = runXStart_;
			// 少し前傾
			worldTransformPlayer_.rotation_.z = -0.20f;
		}
	} break;

	case Phase::kStartRun: {
		// 走り抜け（左→右）
		runTime_ += 1.0f / 60.0f;
		float t = std::clamp(runTime_ / kRunDuration, 0.0f, 1.0f);
		float e = EaseOutCubic(t);

		// X を補間
		worldTransformPlayer_.translation_.x = runXStart_ + (runXEnd_ - runXStart_) * e;

		// ちょい上下に弾ませる
		worldTransformPlayer_.translation_.y = basePlayerY_ + 0.8f * std::sin(e * 2.0f * std::numbers::pi_v<float> * 3.0f);

		// 終了でフェードアウトへ
		if (t >= 1.0f) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
	} break;

	case Phase::kFadeOut: {
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
	} break;
	}

	// タイトル文字の上下揺れ（常時）
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);
	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;
	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	if (skydome_) {
		skydome_->Update();
	}

	// 行列転送
	camera_.TransferMatrix();
	WorldTransformUpdate(worldTransformTitle_);
	WorldTransformUpdate(worldTransformPlayer_);
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	if (skydome_) {
		skydome_->Draw();
	}
	if (modelTitle_) {
		modelTitle_->Draw(worldTransformTitle_, camera_);
	}
	if (modelPlayer_) {
		modelPlayer_->Draw(worldTransformPlayer_, camera_);
	}
	Model::PostDraw();

	if (fade_) {
		fade_->Draw();
	}
}
