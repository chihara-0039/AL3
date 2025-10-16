#define NOMINMAX
#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>
#include <algorithm>

using namespace KamataEngine;


// GameScene.cpp 先頭（include群のあと）に追加
static KamataEngine::Model* TryLoadAnyModel(const std::initializer_list<const char*>& names) {
	for (auto* name : names) {
		auto* m = KamataEngine::Model::CreateFromOBJ(name, true);
		if (m) {
			std::string s = std::string("[Model] loaded: ") + name + "\n";
			OutputDebugStringA(s.c_str());
			return m;
		} else {
			std::string s = std::string("[Model] failed: ") + name + "\n";
			OutputDebugStringA(s.c_str());
		}
	}
	return nullptr;
}



static bool KeyDown(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

bool GameScene::KeyPressed(int vk) {
	static uint8_t latch[256] = {};
	bool now = KeyDown(vk), was = latch[vk] != 0;
	latch[vk] = now ? 1 : 0;
	return now && !was;
}

void GameScene::Initialize() {
	player_.Initialize({0, 0, 0});

	// エンジン Camera 初期化
	camera_.Initialize();
	camera_.translation_ = {0.0f, 2.5f, -6.5f};
	camera_.rotation_ = {0.25f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// モデルロード（block→無ければcube）
	modelPlayer_ = TryLoadAnyModel({"block", "Block", "cube", "Cube", "player", "Player", "unit_cube"});
	if (!modelPlayer_) {
		OutputDebugStringA("[GameScene] ERROR: failed to load any candidate model.\n");
	}
	wtPlayer_ = std::make_unique<WorldTransform>();
	wtPlayer_->Initialize();
	wtPlayer_->scale_ = {1.2f, 1.2f, 1.2f}; // 大きめ
	wtPlayer_->translation_ = {0.0f, 0.0f, 0.0f};
	wtPlayer_->TransferMatrix();

	// 床タイル群
	floorTiles_.clear();
	floorTiles_.reserve((floorHalf_ * 2 + 1) * (floorHalf_ * 2 + 1));

	for (int z = -floorHalf_; z <= floorHalf_; ++z) {
		for (int x = -floorHalf_; x <= floorHalf_; ++x) {
			auto wt = std::make_unique<WorldTransform>();
			wt->Initialize();
			wt->scale_ = {1.0f, floorThin_, 1.0f};                                   // 薄いブロック
			wt->translation_ = {x * floorStep_, -floorThin_ * 0.5f, z * floorStep_}; // 天面を y=0 に合わせる
			wt->TransferMatrix();
			floorTiles_.push_back(std::move(wt));
		}
	}

}

void GameScene::Update() {

	// GameScene.cpp：Update() の最初に F1 トグルを追加
	if ((GetAsyncKeyState(VK_F1) & 0x8000) && !(GetAsyncKeyState(VK_F1) & 0x1)) {
		debugView_ = !debugView_;
		OutputDebugStringA(debugView_ ? "[DebugView] ON\n" : "[DebugView] OFF\n");
	}


	// 入力カメラ更新
	camCtrl_.SetTarget(player_.Position());
	camCtrl_.Update(deltaTime_);

	// プレイヤー更新（カメラ基底で移動）
	Vec3 fwd, right;
	camCtrl_.GetBasis(fwd, right);
	if (KeyPressed('C'))
		player_.SwitchAttackMode();
	if (KeyDown(VK_LBUTTON))
		(void)player_.TryConsumeAmmo();
	player_.Update(deltaTime_, fwd, right);

	// オービットの結果をエンジン Camera に反映
	Vec3 eye, rot;
	camCtrl_.ComputeEyeAndRot(eye, rot);
	camera_.translation_ = {eye.x, eye.y, eye.z};
	camera_.rotation_ = {rot.x, rot.y, rot.z};
	camera_.UpdateMatrix();

	// プレイヤーWTを毎フレ転送
	const Vec3 p = player_.Position();
	wtPlayer_->translation_ = {p.x, p.y, p.z};
	wtPlayer_->TransferMatrix();

	if (debugView_) {
		// 強制的に“絶対見える”配置に
		// カメラ：原点を見る Z-方向からの視点
		camera_.translation_ = {0.0f, 2.0f, -8.0f};
		camera_.rotation_ = {0.0f, 0.0f, 0.0f};
		camera_.UpdateMatrix();

		// モデル：原点・デカめ・回転なし
		wtPlayer_->scale_ = {3.0f, 3.0f, 3.0f};
		wtPlayer_->translation_ = {0.0f, 0.0f, 0.0f};
		wtPlayer_->TransferMatrix();
	}

}

void GameScene::Draw() {
	// DX12 コマンドリストを Model に渡す（必須）
	auto* dx = KamataEngine::DirectXCommon::GetInstance();
	auto* cmdList = dx->GetCommandList();
	KamataEngine::Model::PreDraw(cmdList);

	 if (modelPlayer_) {
		// 床（全タイル）
		for (auto& wt : floorTiles_) {
			modelPlayer_->Draw(*wt, camera_);
		}
		// プレイヤー
		if (wtPlayer_) {
			modelPlayer_->Draw(*wtPlayer_, camera_);
		}
	} else {
		OutputDebugStringA("[Draw] modelPlayer_ is null\n");
	}

	if (modelPlayer_ && wtPlayer_) {
		modelPlayer_->Draw(*wtPlayer_, camera_);
		// 毎60フレームに一度だけ簡易ログ
		if ((frameCounter_++ % 60) == 0) {
			char buf[256];
			auto p = player_.Position();
			snprintf(
			    buf, sizeof(buf), "[Draw] OK frame=%d  cam=(%.2f,%.2f,%.2f)  player=(%.2f,%.2f,%.2f)  dbg=%d\n", frameCounter_, camera_.translation_.x, camera_.translation_.y, camera_.translation_.z,
			    p.x, p.y, p.z, debugView_ ? 1 : 0);
			OutputDebugStringA(buf);
		}
	}

	KamataEngine::Model::PostDraw();
}