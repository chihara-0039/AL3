#include "GameScene.h"
#include "Math.h"
#include <cmath> // fmod
#include <algorithm>

using namespace KamataEngine;

void GameScene::CreateEffect(const Vector3& position) {
	HitEffect* e = HitEffect::Create(position);
	hitEffects_.push_back(e);
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;

	delete block_model_;
	for (auto& line : worldTransformBlocks_) {
		for (WorldTransform* wt : line) {
			delete wt;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipField_;

	for (Enemy* en : enemies_) {
		delete en;
	}
	enemies_.clear();
	delete enemy_model_;

	delete deathParticles_;
	delete deathParticle_model_;

	for (HitEffect* he : hitEffects_) {
		delete he;
	}
	hitEffects_.clear();
	delete particle_model_;

	delete player_;
	delete player_model_;
	delete modelAttack_;

	delete CController_;
	delete fade_;

	//HPバー
	delete hpBackSprite_;
	delete hpFillSprite_;
	delete pauseOverlay_;

	//ポーズセット
	delete escHintSprite_;
	delete pauseMenuSprite_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	worldTransform_.Initialize();

	camera_.Initialize();

	block_model_ = Model::CreateFromOBJ("block");

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// Player
	player_ = new Player();
	player_model_ = Model::CreateFromOBJ("player");
	modelAttack_ = Model::CreateFromOBJ("attack_effect");
	Vector3 playerPos = mapChipField_->GetMapChipPositionByIndex(2, 18);
	player_->SetMapChipField(mapChipField_);
	player_->Initialize(player_model_, modelAttack_, &camera_, playerPos);

	// Camera control
	CController_ = new CameraController();
	CController_->Initialize(&camera_);
	CController_->SetTarget(player_);
	CController_->Reset();
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CController_->SetMovableArea(cameraArea);

	// Enemies
	enemy_model_ = Model::CreateFromOBJ("enemy");
	for (int32_t i = 0; i < 12; ++i) {
		Enemy* e = new Enemy();
		Vector3 pos = mapChipField_->GetMapChipPositionByIndex(30 + i * 5, 18);
		e->Initialize(enemy_model_, &camera_, pos);
		e->SetGameScene(this);
		e->SetMapChipField(mapChipField_);
		e->SetPlayer(player_);
		enemies_.push_back(e);
	}

	// Effects
	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");
	particle_model_ = Model::CreateFromOBJ("particle");
	HitEffect::SetModel(particle_model_);
	HitEffect::SetCamera(&camera_);

	// Fade
	phase_ = Phase::kFadeIn;
	result_ = Result::kNone;
	finished_ = false;

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// HP bar
	hpTexture_ = TextureManager::Load("white1x1.png"); // 1px白
	hpBackSprite_ = Sprite::Create(hpTexture_, {hpPos_.x, hpPos_.y});
	hpFillSprite_ = Sprite::Create(hpTexture_, {hpPos_.x, hpPos_.y});
	if (hpBackSprite_) {
		hpBackSprite_->SetSize({hpWidth_, hpHeight_});
		hpBackSprite_->SetColor({0.0f, 0.0f, 0.0f, 0.5f});
	}
	if (hpFillSprite_) {
		hpFillSprite_->SetSize({hpWidth_, hpHeight_});
		hpFillSprite_->SetColor({1.0f, 0.2f, 0.2f, 1.0f});
	}

	// === 画像ベースのUI ===
	// プレイ中の右上ヒント（ESCでポーズ）
	escHintTexture_ = TextureManager::Load("gameUI/ui_esc_pause.png");
	escHintSprite_ = Sprite::Create(escHintTexture_, {(float)WinApp::kWindowWidth - 20.0f, 20.0f});
	if (escHintSprite_) {
		escHintSprite_->SetAnchorPoint({1.0f, 0.0f}); // 右上基準
	}

	// ポーズメニュー（中央）
	pauseMenuTexture_ = TextureManager::Load("gameUI/ui_pause_menu.png");
	pauseMenuSprite_ = Sprite::Create(pauseMenuTexture_, {(float)WinApp::kWindowWidth * 0.5f, (float)WinApp::kWindowHeight * 0.5f});
	if (pauseMenuSprite_) {
		pauseMenuSprite_->SetAnchorPoint({0.5f, 0.5f}); // 中央基準
	}

	// Pause overlay（使うなら）
	pauseOverlay_ = Sprite::Create(hpTexture_, {0.0f, 0.0f});
	if (pauseOverlay_) {
		pauseOverlay_->SetSize({(float)WinApp::kWindowWidth, (float)WinApp::kWindowHeight});
		pauseOverlay_->SetColor({0.0f, 0.0f, 0.0f, 0.5f});
	}

	// 死亡アニメ初期化
	wtDeathPlayer_.Initialize();
	drawDeathPlayer_ = false;
	deathAnimT_ = 0.0f;
	deathBaseY_ = 0.0f;
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			// フェーズ遷移
			phase_ = Phase::kDeath;
			result_ = Result::kGameOver;

			// ★倒れアニメの初期化をここに移動
			wtDeathPlayer_.Initialize();
			const Vector3 pos = player_->GetWorldPosition();
			wtDeathPlayer_.translation_ = pos;
			wtDeathPlayer_.rotation_ = {0.0f, 0.0f, 0.0f};
			wtDeathPlayer_.scale_ = {1.0f, 1.0f, 1.0f}; // 必要ならプレイヤーの実スケールに
			deathBaseY_ = pos.y;
			deathAnimT_ = 0.0f;
			drawDeathPlayer_ = true;
			WorldTransformUpdate(wtDeathPlayer_); // ★初期フレームから描けるように

			// デスパーティクル
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, pos);
		}
		break;

	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
			if (fade_) {
				fade_->Start(Fade::Status::FadeOut, 1.0f);
			}
		}
		break;
	default:
		break;
	}
}


void GameScene::GenerateBlocks() {
	uint32_t v = mapChipField_->GetNumBlockVirtical();
	uint32_t h = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(v);
	for (uint32_t i = 0; i < v; ++i) {
		worldTransformBlocks_[i].resize(h);
	}

	for (uint32_t i = 0; i < v; ++i) {
		for (uint32_t j = 0; j < h; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* wt = new WorldTransform();
				wt->Initialize();
				wt->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				worldTransformBlocks_[i][j] = wt;
			}
		}
	}
}

void GameScene::Update() {
	// エフェクト掃除
	hitEffects_.remove_if([](HitEffect* h) {
		if (h->IsDead()) {
			delete h;
			return true;
		}
		return false;
	});

	// 倒した敵の開放
	enemies_.remove_if([](Enemy* e) {
		if (e->IsDead()) {
			delete e;
			return true;
		}
		return false;
	});

	// ★全敵撃破 → クリアへ
	if (phase_ == Phase::kPlay && enemies_.empty()) {
		result_ = Result::kClear;
		phase_ = Phase::kFadeOut;
		if (fade_) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
	}

	// ★先にフェーズ判定（ここで kDeath に入る）
	ChangePhase();

	// ★倒れアニメは kDeath と kFadeOut の間、ずっと進める
	if (drawDeathPlayer_ && (phase_ == Phase::kDeath || phase_ == Phase::kFadeOut)) {
		UpdateDeathAnim(1.0f / 60.0f);
	}

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}
		skydome_->Update();
		CController_->Update();
		player_->Update();
		for (Enemy* e : enemies_) {
			e->Update();
		}
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			camera_.TransferMatrix();
		} else {
			camera_.UpdateMatrix();
		}
		for (auto& line : worldTransformBlocks_) {
			for (WorldTransform* wt : line) {
				if (wt)
					WorldTransformUpdate(*wt);
			}
		}
		for (HitEffect* h : hitEffects_) {
			h->Update();
		}
		break;

	case Phase::kPlay:
		// ポーズ
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			phase_ = Phase::kPause;
			break;
		}
		skydome_->Update();
		CController_->Update();
		player_->Update();
		for (Enemy* e : enemies_) {
			e->Update();
		}

		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			camera_.TransferMatrix();
		} else {
			camera_.UpdateMatrix();
		}
		for (auto& line : worldTransformBlocks_) {
			for (WorldTransform* wt : line) {
				if (wt)
					WorldTransformUpdate(*wt);
			}
		}
		CheckAllCollisions();
		for (HitEffect* h : hitEffects_) {
			h->Update();
		}
		break;

	case Phase::kDeath:
		skydome_->Update();
		CController_->Update();
		for (Enemy* e : enemies_) {
			e->Update();
		}
		if (deathParticles_) {
			deathParticles_->Update();
		}
		for (HitEffect* h : hitEffects_) {
			h->Update();
		}
		// UpdateDeathAnim は上で毎フレ呼んでる
		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		skydome_->Update();
		CController_->Update();
		for (Enemy* e : enemies_) {
			e->Update();
		}
		for (HitEffect* h : hitEffects_) {
			h->Update();
		}
		// UpdateDeathAnim は上で毎フレ呼んでる
		break;

	case Phase::kPause:
		UpdatePause();
		camera_.UpdateMatrix();
		break;
	}

	uiTimer_ += 1.0f / 60.0f;
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3D
	Model::PreDraw(dxCommon->GetCommandList());

	// ★通常 or 倒れモデルのどちらか一方だけ描画
	if (drawDeathPlayer_) {
		if (player_model_) {
			player_model_->Draw(wtDeathPlayer_, camera_);
		}
	} else {
		if (!player_->IsDead()) {
			player_->Draw();
		}
	}

	skydome_->Draw();

	for (auto& line : worldTransformBlocks_) {
		for (WorldTransform* wt : line) {
			if (wt)
				block_model_->Draw(*wt, camera_);
		}
	}
	for (Enemy* e : enemies_) {
		e->Draw();
	}
	if (deathParticles_) {
		deathParticles_->Draw();
	}
	for (HitEffect* h : hitEffects_) {
		h->Draw();
	}

	Model::PostDraw();

	// 2D
	Sprite::PreDraw(dxCommon->GetCommandList());
	if (!player_->IsDead()) {
		DrawHPBar();
	}
	if (phase_ == Phase::kPlay && escHintSprite_) {
		escHintSprite_->Draw();
	}
	if (phase_ == Phase::kPause) {
		DrawPauseOverlay();
		if (pauseMenuSprite_) {
			pauseMenuSprite_->Draw();
		}
	}
	Sprite::PostDraw();

	if (fade_) {
		fade_->Draw();
	}
}


void GameScene::DrawHPBar() {
	if (!hpBackSprite_ || !hpFillSprite_ || !player_)
		return;

	const int hp = player_->GetHP();
	const int mh = player_->GetMaxHP();
	float ratio = (mh > 0) ? (float)hp / (float)mh : 0.0f;
	ratio = std::clamp(ratio, 0.0f, 1.0f);

	hpBackSprite_->SetPosition({hpPos_.x, hpPos_.y});
	hpBackSprite_->SetSize({hpWidth_, hpHeight_});
	hpBackSprite_->SetColor({0.0f, 0.0f, 0.0f, 0.5f});
	hpBackSprite_->Draw();

	hpFillSprite_->SetPosition({hpPos_.x, hpPos_.y});
	hpFillSprite_->SetSize({hpWidth_ * ratio, hpHeight_});
	if (player_->IsInvincible()) {
		bool on = (std::fmod(uiTimer_, 0.1f) < 0.05f);
		float a = on ? 1.0f : 0.35f;
		hpFillSprite_->SetColor({1.0f, 1.0f, 0.0f, a});
	} else {
		hpFillSprite_->SetColor({1.0f, 0.2f, 0.2f, 1.0f});
	}
	hpFillSprite_->Draw();
}

void GameScene::DrawUI() {
	// （ImGuiを使うならここで描く。今回は不要）
}

void GameScene::UpdatePause() {
	//タイトルへ戻る（確認OK）
	if (Input::GetInstance()->TriggerKey(DIK_Y) || Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		result_ = Result::kReturnToTitle;
		phase_ = Phase::kFadeOut;
		if (fade_) {
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
		return;
	}
	// N or ESC → ゲーム続行
	if (Input::GetInstance()->TriggerKey(DIK_N) || Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		phase_ = Phase::kPlay;
	}
}
void GameScene::DrawPauseOverlay() {
	if (!pauseOverlay_) {
		return;
	}
	// 画面全体に黒半透明を出すだけ（“PAUSE / Y:Title  N:Resume”の文字は
	// DebugTextやテクスチャを足せるならお好みで）
	pauseOverlay_->Draw();
}

void GameScene::CheckAllCollisions() {
	AABB aabb1 = player_->GetAABB();
	for (Enemy* enemy : enemies_) {
		if (enemy->IsCollisionDisabled())
			continue;
		AABB aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}
}

void GameScene::UpdateDeathAnim(float dt) {
	deathAnimT_ += dt;
	const float dur = 0.6f; // 0.6秒で倒れる
	float t = std::clamp(deathAnimT_ / dur, 0.0f, 1.0f);
	float e = EaseOutCubic(t);

	// 前のめりに倒れる（Z回転）+ 少し沈む
	float fallAngle = 1.40f * e; // 約80度
	float drop = 1.0f * e;       // 1.0ユニット下げる

	wtDeathPlayer_.rotation_.z = +fallAngle;
	wtDeathPlayer_.translation_.y = deathBaseY_ - drop;

	WorldTransformUpdate(wtDeathPlayer_);
}
