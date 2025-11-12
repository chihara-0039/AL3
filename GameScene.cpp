#include "GameScene.h"
#include "KamataEngine.h"
#include "Math.h"
#include <Windows.h>

using namespace KamataEngine;

//マウスクリックを検出
namespace {
	bool LeftClickTriggered() { 
		static bool wasDown = false;
	    const bool isDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	    const bool triggered = isDown && !wasDown;
	    wasDown = isDown;
	    return triggered;
	}
}



void GameScene::Initialize() {

	// モデルのロード
	player_model_ = Model::CreateFromOBJ("player");

	// カメラ初期化
	camera_.Initialize();

	// カメラ位置をプレイヤーに近づける
	camera_.translation_ = {0.0f, 0.0f, -10.0f}; // Y:高さ、Z:奥行き
	camera_.UpdateMatrix();                      // 行列更新

	// デバッグカメラ作成
	debugCamera_ = new DebugCamera(1280, 720);

	// プレイヤー初期化（座標など）
	player_ = new Player();
	player_->Initialize(player_model_, &camera_, {0.0f, 0.0f, 0.0f});
}

void GameScene::Update() {

#ifdef _DEBUG
	//// スペースキーでデバッグカメラ切り替え
	// if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//	isDebugCameraActive_ = !isDebugCameraActive_;
	// }
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	// プレイヤー更新
	player_->Update();

	//=== マウス照準で撃つ（←クリックで発射・深度アリ）＝＝＝
	const float kWindowWith = 1280.0f;
	const float kWindowHeight = 720.0f;
	

	//マウス位置取得
	Vector2 mouse{};
	if (Input::GetInstance()) {
		mouse = Input::GetInstance()->GetMousePosition(); //(px, px) 例：左上(0,0) 右下(1280,720)

	}

	// スクリーン座標をNDCに変換
	const float ndcX = (mouse.x / kWindowWith) * 2.0f - 1.0f;
	const float ndcY = -((mouse.y / kWindowHeight) * 2.0f - 1.0f); // Y座標は上下逆

	// View*Projectionの逆行列を計算
	Matrix4x4 vp = camera_.matView * camera_.matProjection; // View→Projection変換行列
	Matrix4x4 invVP = Inverse(vp);                          // 逆行列

	// NDC→ワールド座標変換
	auto Unproject = [&](float x, float y, float z01) -> Vector3 {
		// 同次座標
		float vx = x, vy = y, vz = z01, vw = 1.0f;
		// 逆行列で変換	(invVP * (x,y,z,1))
		Vector3 out{};
		float ox = vx * invVP.m[0][0] + vy * invVP.m[1][0] + vz * invVP.m[2][0] + vw * invVP.m[3][0];
		float oy = +vx * invVP.m[0][1] + vy * invVP.m[1][1] + vz * invVP.m[2][1] + vw * invVP.m[3][1];
		float oz = +vx * invVP.m[0][2] + vy * invVP.m[1][2] + vz * invVP.m[2][2] + vw * invVP.m[3][2];
		float ow = +vx * invVP.m[0][3] + vy * invVP.m[1][3] + vz * invVP.m[2][3] + vw * invVP.m[3][3];
		if (std::fabs(ow) < 1e-8f)
			ow = 1.0f;
		out.x = ox / ow;
		out.y = oy / ow;
		out.z = oz / ow;
		return out;
	};

	const Vector3 nearWorld = Unproject(ndcX, ndcY, 0.0f); // クリップ空間の手前
	const Vector3 farWorld = Unproject(ndcX, ndcY, 1.0f);                                                      // クリップ空間の奥
	Vector3 rayDir = Normalized(farWorld - nearWorld);                                                         // レイ方向
	                                                                                                         // 弾はレイ上の充分遠い点へ向けて撃つ（既存の FireToward を活用）
	const float kShootDepth = 100.0f;                                                                        // “奥行き”の飛ばし量（調整用）
	Vector3 targetWorld = nearWorld + rayDir * kShootDepth;



	//発射トリガー
	if (LeftClickTriggered()) {
		player_->FireToward(targetWorld);
	}
}

void GameScene::Draw() {

	// DirectX共通処理取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// プレイヤー描画
	player_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();
}

void GameScene::Delete() {

	delete player_;
	delete player_model_;
	delete debugCamera_;
}
