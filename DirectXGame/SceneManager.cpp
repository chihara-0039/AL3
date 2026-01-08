#include "SceneManager.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

namespace {
bool IsSpaceDown() { return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0; }
} // namespace

void SceneManager::Initialize(SceneName start) {
	if (isInitialized_) {
		return;
	}
	ApplySceneChange(start);
	isInitialized_ = true;
}

void SceneManager::ChangeScene(SceneName next) {
	// 未初期化なら、まず初期化扱いで切り替え
	if (!isInitialized_) {
		Initialize(next);
		return;
	}

	// 同じシーンへの要求は無視（点滅/もっさり対策）
	if (next == currentName_) {
		return;
	}

	// すでに同じ要求が積まれてるなら何もしない
	if (hasPendingChange_ && pendingName_ == next) {
		return;
	}

	hasPendingChange_ = true;
	pendingName_ = next;
}

void SceneManager::ApplySceneChange(SceneName next) {
	// 旧シーン終了
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}

	currentName_ = next;

	// 新シーン生成
	switch (next) {
	case SceneName::TITLE:
		currentScene_ = std::make_unique<TitleScene>();
		break;
	case SceneName::GAME:
		currentScene_ = std::make_unique<GameScene>();
		break;
	case SceneName::CLEAR:
		currentScene_ = std::make_unique<ClearScene>();
		break;
	case SceneName::GAMEOVER:
		currentScene_ = std::make_unique<GameOverScene>();
		break;
	}

	// 初期化
	if (currentScene_) {
		currentScene_->Initialize();
	}

	// 押しっぱなしで次シーンに入った場合、離すまでUpdateを止める
	waitSpaceRelease_ = IsSpaceDown();
}

void SceneManager::Update() {
	// 念のため：呼び忘れてても動く
	if (!isInitialized_) {
		Initialize(SceneName::TITLE);
	}

	// Spaceが離されるまで Update を呼ばない（入力押しっぱなし対策）
	if (waitSpaceRelease_) {
		if (IsSpaceDown()) {
			return; // Drawは通常どおり呼ばれる想定
		}
		waitSpaceRelease_ = false;
	}

	// 現在シーン更新
	if (currentScene_) {
		currentScene_->Update();
	}

	// Updateの最後に安全に切り替える（Update中にdeleteしない）
	if (hasPendingChange_) {
		const SceneName next = pendingName_;
		hasPendingChange_ = false;
		ApplySceneChange(next);
	}
}

void SceneManager::Draw() {
	if (!currentScene_) {
		return;
	}

	// ※dxCommon->PreDraw()/PostDraw() は main 側で 1フレーム1回だけ呼ぶ
	auto* dxCommon = DirectXCommon::GetInstance();
	auto* cmdList = dxCommon->GetCommandList();

	// --- 3D ---
	Model::PreDraw(cmdList);
	currentScene_->Draw3D();
	Model::PostDraw();

	// --- 2D ---
	Sprite::PreDraw(cmdList);
	currentScene_->Draw2D();
	Sprite::PostDraw();
}

void SceneManager::Finalize() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
	hasPendingChange_ = false;
	waitSpaceRelease_ = false;
	isInitialized_ = false;
}
