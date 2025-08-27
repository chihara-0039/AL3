#include "GameClearScene.h"
#include "Math.h"
#include <cmath> // ★ 追加：std::fmod, std::sin 用
#include <numbers>

GameClearScene::~GameClearScene() {
	delete modelPlayer_;
	delete modelGameClear_;
	delete skydome_;
	delete modelSkydome_;
	delete dimOverlay_;
}

void GameClearScene::Initialize() {
	// 文字モデル（プロジェクトの名前に合わせて）
	modelGameClear_ = Model::CreateFromOBJ("GameClearFont", true);
	modelPlayer_	= Model::CreateFromOBJ("Player", true);

	camera_.Initialize();

	 // 追加：スカイドーム
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelSkydome_, &camera_);

	// 追加：画面暗転オーバーレイ（ポーズ時と同じ雰囲気）
	overlayTex_ = TextureManager::Load("white1x1.png");
	dimOverlay_ = Sprite::Create(overlayTex_, {0.0f, 0.0f});
	if (dimOverlay_) {
		dimOverlay_->SetSize({(float)WinApp::kWindowWidth, (float)WinApp::kWindowHeight});
		dimOverlay_->SetColor({0.0f, 0.0f, 0.0f, 0.5f}); // 半透明黒
	}


	const float kTitleScale = 2.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTitleScale, kTitleScale, kTitleScale};

	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -10.0f;

	basePlayerY_ = worldTransformPlayer_.translation_.y;
	cheerTime_ = 0.0f;
}

void GameClearScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	// 追加：スカイドーム更新
	if (skydome_) {
		skydome_->Update();
	}

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeFontMove);
	float angle = counter_ / kTimeFontMove * 2.0f * std::numbers::pi_v<float>;
	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	// ★喜びアニメ：小ジャンプ＋回転＋微スケール
	cheerTime_ += 1.0f / 60.0f;
	float w = 2.0f * std::numbers::pi_v<float> * 1.6f;                // 1.6Hz くらいで跳ねる
	float hop = (std::max)(0.0f, std::sin(cheerTime_ * w));             // 下には行かない小ジャンプ
	worldTransformPlayer_.translation_.y = basePlayerY_ + hop * 1.2f; // 振幅1.2

	worldTransformPlayer_.rotation_.y += 0.05f; // ゆっくり回る

	float pulse = 1.0f + 0.03f * std::sin(cheerTime_ * 2.0f * std::numbers::pi_v<float> * 2.0f);
	worldTransformPlayer_.scale_ = {10.0f * pulse, 10.0f * pulse, 10.0f * pulse};

	camera_.TransferMatrix();
	WorldTransformUpdate(worldTransformTitle_);
	WorldTransformUpdate(worldTransformPlayer_);
}

void GameClearScene::Draw() {
	DirectXCommon* dx = DirectXCommon::GetInstance();
	auto* cl = dx->GetCommandList();

	// 3D（背景にスカイドーム）
	Model::PreDraw(cl);
	if (skydome_)
		skydome_->Draw();
	if (modelGameClear_)
		modelGameClear_->Draw(worldTransformTitle_, camera_);
	if (modelPlayer_)
		modelPlayer_->Draw(worldTransformPlayer_, camera_);
	Model::PostDraw();

	// 暗転は画面全体に重ねるのでスプライトで最後に
	Sprite::PreDraw(cl);
	if (dimOverlay_)
		dimOverlay_->Draw();
	Sprite::PostDraw();
}