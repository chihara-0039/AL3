#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "HitEffect.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "skydome.h"
#include <list>
#include <vector>

class GameScene {
public:
	~GameScene();
	void Initialize();
	void Update();
	void Draw();
	void GenerateBlocks();
	void DrawUI(); // （ImGui用：必要なら）

	void CheckAllCollisions();

	bool IsFinished() const { return finished_; }

	enum class Result {
		kNone,
		kGameOver,      // プレイヤー死亡
		kClear,         // 敵全滅
		kReturnToTitle, // （将来）ポーズからタイトル
	};
	Result GetResult() const { return result_; }

	void CreateEffect(const Vector3& position);

private:
	enum class Phase { kFadeIn, kPlay, kDeath, kPause, kFadeOut };
	Phase phase_;

	void ChangePhase();

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	Player* player_ = nullptr;
	KamataEngine::Model* player_model_ = nullptr;

	KamataEngine::Model* block_model_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	CameraController* CController_ = nullptr;

	KamataEngine::Model* enemy_model_ = nullptr;
	std::list<Enemy*> enemies_;

	DeathParticles* deathParticles_ = nullptr;
	Model* deathParticle_model_ = nullptr;

	bool finished_ = false;
	Result result_ = Result::kNone;

	Fade* fade_ = nullptr;

	Model* modelAttack_ = nullptr;

	std::list<HitEffect*> hitEffects_;
	Model* particle_model_ = nullptr;

	void SetMapChipField(MapChipField* field) { mapChipField_ = field; }

	// HPバー（ImGuiなし）
	KamataEngine::Sprite* hpBackSprite_ = nullptr;
	KamataEngine::Sprite* hpFillSprite_ = nullptr;
	uint32_t hpTexture_ = 0;

	Vector2 hpPos_ = {20.0f, 20.0f};
	float hpWidth_ = 220.0f;
	float hpHeight_ = 18.0f;

	float uiTimer_ = 0.0f;
	void DrawHPBar();

	// ポーズ系（今回は未実装でもOK）
	bool pause_ = false;
	KamataEngine::Sprite* pauseOverlay_ = nullptr;
	void UpdatePause();
	void DrawPauseOverlay();

	 // ===== 画像ベースのUI =====
	// プレイ中の右上ヒント（ESCでポーズ）
	uint32_t escHintTexture_ = 0;
	KamataEngine::Sprite* escHintSprite_ = nullptr; // ui_esc_pause.png

	// ポーズメニュー（中央）
	uint32_t pauseMenuTexture_ = 0;
	KamataEngine::Sprite* pauseMenuSprite_ = nullptr; // ui_pause_menu.png

	// 死亡アニメ用（プレイヤー本体は IsDead() で描画されないので別Transformで描く）
	KamataEngine::WorldTransform wtDeathPlayer_;
	bool drawDeathPlayer_ = false;
	float deathAnimT_ = 0.0f;
	float deathBaseY_ = 0.0f;

	// 補助
	static float EaseOutCubic(float t) { return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t); }
	void UpdateDeathAnim(float dt);

};
