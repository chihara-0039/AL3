#include "GameScene.h"
#include "KamataEngine.h"
#include "Math.h"
#include <Windows.h>
#include <cmath>

using namespace KamataEngine;

// 左クリック長押し検出（押下状態）
namespace {
inline bool LeftDown() { return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0; }
} // namespace

void GameScene::Initialize() {

	// モデルのロード
	player_model_ = Model::CreateFromOBJ("player1");
	// プレイヤー弾モデルのロード
	playerBullet_model_ = Model::CreateFromOBJ("PlayerBullet");

	// 敵モデルのロード
	enemy_model_ = Model::CreateFromOBJ("Boss");

	// ファンネル本体のモデル
	funnel_model_ = Model::CreateFromOBJ("funnel");
	// ファンネル弾モデルのロード
	funnelBullet_model_ = KamataEngine::Model::CreateFromOBJ("funnel");

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.UpdateMatrix();

	// スカイドーム初期化
	skydome_ = new Skydome();
	skydome_->Initialize(&camera_);
	skydome_->SetRadius(500.0f);

	// デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);

	// プレイヤー初期化
	player_ = new Player();
	player_->Initialize(player_model_, playerBullet_model_, &camera_, {0.0f, 0.0f, 0.0f});

	// ★ 敵初期化（画面奥に1体）
	enemy_ = new Enemy();
	enemy_->Initialize(enemy_model_, &camera_, {0.0f, 0.0f, 30.0f});

	// ファンネルの回転中心（画面奥）
	Vector3 funnelCenter = {0.0f, 0.0f, 0.0f};

	const int kFunnelCount = 3;
	const float kFunnelRadius = 6.0f;

	for (int i = 0; i < kFunnelCount; ++i) {
		float angle = ToRadians(360.0f / kFunnelCount * i); // 均等配置
		Funnel* funnel = new Funnel();
		funnel->Initialize(funnel_model_, &camera_, funnelCenter, kFunnelRadius, angle);
		funnels_.push_back(funnel);
	}

	// 攻撃タイマー初期値
	funnelAttackTimer_ = 60; // 1秒後に最初の攻撃
}

void GameScene::Update() {

#ifdef _DEBUG
	// デバッグカメラ切り替えなど…
#endif

	// === レール前進カメラ ===
	// const float kScrollSpeed = 0.2f; // お好みで
	// camera_.translation_.z += kScrollSpeed;
	// camera_.UpdateMatrix();

	// スカイドーム
	skydome_->Update();

	if (isDebugCameraActive_) {
		debugCamera_->Update();

		const Camera& dbg = debugCamera_->GetCamera();
		camera_.matView = dbg.matView;
		camera_.matProjection = dbg.matProjection;

		// matView からカメラ位置を復元（Skydome が translation_ を使うため）
		Matrix4x4 invView = Inverse(camera_.matView);
		camera_.translation_ = {invView.m[3][0], invView.m[3][1], invView.m[3][2]};

		camera_.TransferMatrix();
	} else {
		// camera_.translation_.z += kScrollSpeed;
		camera_.UpdateMatrix();
	}

	// プレイヤー更新
	player_->Update();

	// 敵更新
	if (enemy_) {
		enemy_->Update();
	}

	// --------- ファンネル本体の更新 ----------
	Vector3 funnelCenter = {0.0f, 0.0f, 30.0f};
	for (Funnel* funnel : funnels_) {
		funnel->Update(funnelCenter);
	}

	// ここでファンネル弾の更新を入れる
	for (FunnelBullet* bullet : funnelBullets_) {
		bullet->Update();
	}

	// プレイヤーとの当たり判定
	Vector3 playerPos = player_->GetWorldPosition();
	float playerR = player_->GetCollisionRadius();
	const int kPlayerHitDamage = 1;

	for (FunnelBullet* bullet : funnelBullets_) {

		if (bullet->IsDead()) {
			continue;
		}

		// すでに無敵中なら判定しなくてよい
		if (player_->IsInvincible() || player_->IsDead()) {
			break;
		}

		Vector3 bpos = bullet->GetWorldPosition();

		Vector3 diff = bpos - playerPos;
		float dist2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
		float r = playerR; // 弾の半径も足すならここで + something

		if (dist2 <= r * r) {
			// 被弾
			player_->OnHit(kPlayerHitDamage);
		}
	}

	// 死んだ弾の削除
	funnelBullets_.remove_if([](FunnelBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// --------- ファンネル攻撃（奥からビーム） ----------
	// パターンタイマー進行
	++patternTimer_;

	// プレイヤー位置（狙い用）
	playerPos = player_->GetWorldPosition();

	switch (currentPattern_) {
	case BossPattern::FunnelGatling: {

		// 例：最初の60Fは準備（何もしない）
		const int kPrepTime = 60;
		const int kFireStartTime = kPrepTime;
		const int kFireEndTime = kPrepTime + 240; // 4秒間撃ちまくる

		const int kGatlingInterval = 5; // 5Fごとに1発
		const float kBulletSpeed = 0.8f;

		if (patternTimer_ >= kFireStartTime && patternTimer_ <= kFireEndTime) {

			// 一定フレームごとに発射
			if ((patternTimer_ % kGatlingInterval) == 0) {

				for (Funnel* funnel : funnels_) {
					Vector3 start = funnel->GetWorldPosition();
					Vector3 dir = Normalized(playerPos - start);
					Vector3 vel = dir * kBulletSpeed;

					FunnelBullet* bullet = new FunnelBullet();
					bullet->Initialize(funnelBullet_model_, start, vel);
					funnelBullets_.push_back(bullet);
				}
			}
		}

		// 終了したら次のパターンへ（ここではまだBeam未実装なのでループ）
		if (patternTimer_ > kFireEndTime) {
			patternTimer_ = 0;
			currentPattern_ = BossPattern::FunnelBeam; // 次のパターンへ
		}

	} break;

	case BossPattern::FunnelBeam:
		// ここに後で「照射ビーム攻撃」を実装する
		// ひとまず何もせず、一定時間経ったらまたガトリングに戻すでもOK
		if (patternTimer_ > 180) {
			patternTimer_ = 0;
			currentPattern_ = BossPattern::FunnelGatling;
		}
		break;
	}

	// （ここに「プレイヤーのマウス照準ショット」の処理が続く）
	//   既存の Unproject → targetWorld → player_->FireToward(targetWorld)
	//   のブロックはそのまま残してOK

	//========================================
	// プレイヤーのマウス照準 5連バースト処理
	//========================================

	const float kWindowWidth = 1280.0f;
	const float kWindowHeight = 720.0f;

	// マウス座標取得（画面左上が(0,0)）
	Vector2 mouse{};
	if (Input::GetInstance()) {
		mouse = Input::GetInstance()->GetMousePosition();
	}

	// スクリーン座標 → NDC(-1～1) へ変換
	const float ndcX = (mouse.x / kWindowWidth) * 2.0f - 1.0f;
	const float ndcY = -((mouse.y / kWindowHeight) * 2.0f - 1.0f); // 上下反転

	// View * Projection の逆行列
	Matrix4x4 vp = camera_.matView * camera_.matProjection;
	Matrix4x4 invVP = Inverse(vp);

	// NDC → ワールド座標
	auto Unproject = [&](float x, float y, float z01) -> Vector3 {
		float vx = x;
		float vy = y;
		float vz = z01;
		float vw = 1.0f;

		Vector3 out{};
		float ox = vx * invVP.m[0][0] + vy * invVP.m[1][0] + vz * invVP.m[2][0] + vw * invVP.m[3][0];
		float oy = vx * invVP.m[0][1] + vy * invVP.m[1][1] + vz * invVP.m[2][1] + vw * invVP.m[3][1];
		float oz = vx * invVP.m[0][2] + vy * invVP.m[1][2] + vz * invVP.m[2][2] + vw * invVP.m[3][2];
		float ow = vx * invVP.m[0][3] + vy * invVP.m[1][3] + vz * invVP.m[2][3] + vw * invVP.m[3][3];

		if (std::fabs(ow) < 1e-8f) {
			ow = 1.0f;
		}

		out.x = ox / ow;
		out.y = oy / ow;
		out.z = oz / ow;
		return out;
	};

	const Vector3 nearWorld = Unproject(ndcX, ndcY, 0.0f);
	const Vector3 farWorld = Unproject(ndcX, ndcY, 1.0f);
	Vector3 rayDir = Normalized(farWorld - nearWorld);

	// レイ上の奥の点を狙い位置とする
	const float kShootDepth = 100.0f;
	Vector3 targetWorld = nearWorld + rayDir * kShootDepth;

	// --- 5連バースト ---
	static int burstCount = 0;      // 残り何発か
	static int intraBurstTimer = 0; // バースト内の弾間隔
	static int burstCooldown = 0;   // バースト終了後のクールタイム

	const int kBurstSize = 5;          // 1バーストの弾数
	const int kIntraBurstInterval = 3; // バースト内の弾間隔（フレーム）
	const int kBurstCooldown = 20;     // バースト終了後のクールタイム（フレーム）

	if (LeftDown()) {
		// まだバーストしてなくてクールタイムも終わってたら新しいバースト開始
		if (burstCount == 0 && burstCooldown == 0) {
			burstCount = kBurstSize;
			intraBurstTimer = 0;
		}
	} else {
		// ボタン離したら即リセット（次押しでまた5発から）
		burstCount = 0;
		intraBurstTimer = 0;
		burstCooldown = 0;
	}

	// クールタイム進行
	if (burstCooldown > 0) {
		--burstCooldown;
	}

	// バースト進行
	if (burstCount > 0) {
		if (intraBurstTimer == 0) {
			// 1発撃つ（マウスカーソル方向）
			player_->FireToward(targetWorld);

			--burstCount;
			intraBurstTimer = kIntraBurstInterval;

			// 撃ち切ったらクールタイム開始
			if (burstCount == 0) {
				burstCooldown = kBurstCooldown;
			}
		} else {
			--intraBurstTimer;
		}
	}

	//=========================
	// プレイヤー弾 vs 敵 の当たり判定
	//=========================
	if (enemy_ && !enemy_->IsDead()) {

		AABB enemyBox = enemy_->GetAABB();

		const auto& bullets = player_->GetBullets();
		for (PlayerBullet* bullet : bullets) {

			if (bullet->IsDead()) {
				continue;
			}

			Vector3 bpos = bullet->GetWorldPosition();
			const float r = 0.5f; // 弾の“半径”。見た目に合わせて調整

			AABB bulletBox;
			bulletBox.min = {bpos.x - r, bpos.y - r, bpos.z - r};
			bulletBox.max = {bpos.x + r, bpos.y + r, bpos.z + r};

			if (IsCollision(enemyBox, bulletBox)) {
				enemy_->OnHit(bullet->GetDamage());
				bullet->OnHit();
			}
		}
	}

	// === ボス撃破チェック ===
	if (enemy_ && enemy_->IsDead()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::CLEAR);
		return; // このフレームの残り処理は要らないので早期リターンでもOK
	}

	// === プレイヤー死亡チェック ===
	if (player_->IsDead()) {
		SceneManager::GetInstance()->ChangeScene(SceneName::GAMEOVER);
		return;
	}
}

void GameScene::Draw3D() {
	// スカイドーム描画
	if (skydome_) {
		skydome_->Draw();
	}

	// プレイヤー
	player_->Draw();

	// 敵
	if (enemy_) {
		enemy_->Draw();
	}

	// ファンネル
	for (Funnel* f : funnels_) {
		f->Draw();
	}

	// ファンネル弾
	for (FunnelBullet* b : funnelBullets_) {
		b->Draw(camera_);
	}
}

void GameScene::Finalize() {

	// プレイヤー解放
	delete player_;
	player_ = nullptr;

	// プレイヤーモデル解放
	delete player_model_;
	player_model_ = nullptr;

	// 敵解放
	delete enemy_;
	delete enemy_model_;
	enemy_ = nullptr;
	enemy_model_ = nullptr;

	for (Funnel* funnel : funnels_) {
		delete funnel;
	}
	funnels_.clear();

	for (FunnelBullet* bullet : funnelBullets_) {
		delete bullet;
	}
	funnelBullets_.clear();

	// ファンネルモデル解放
	delete funnel_model_;
	funnel_model_ = nullptr;

	// ファンネル弾モデル解放
	delete funnelBullet_model_;
	funnelBullet_model_ = nullptr;

	// スカイドーム解放
	delete skydome_;
	skydome_ = nullptr;

	// デバッグカメラ解放
	delete debugCamera_;
	debugCamera_ = nullptr;
}
