#include "GameClearScene.h" // ★追加
#include "GameOverScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
GameOverScene* gameOverScene = nullptr;
GameClearScene* gameClearScene = nullptr; // ★追加

enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kGameOver,
	kGameClear, // ★追加
};
Scene scene = Scene::kUnknown;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (gameScene->IsFinished()) {
			switch (gameScene->GetResult()) {
			case GameScene::Result::kGameOver:
				scene = Scene::kGameOver;
				delete gameScene;
				gameScene = nullptr;
				gameOverScene = new GameOverScene;
				gameOverScene->Initialize();
				break;
			case GameScene::Result::kClear:
				scene = Scene::kGameClear; // ★クリアへ
				delete gameScene;
				gameScene = nullptr;
				gameClearScene = new GameClearScene;
				gameClearScene->Initialize();
				break;
			case GameScene::Result::kReturnToTitle:
			default:
				scene = Scene::kTitle;
				delete gameScene;
				gameScene = nullptr;
				titleScene = new TitleScene;
				titleScene->Initialize();
				break;
			}
		}
		break;

	case Scene::kGameOver:
		if (gameOverScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;

	case Scene::kGameClear: // ★追加
		if (gameClearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;

	default:
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameOver:
		gameOverScene->Update();
		break;
	case Scene::kGameClear:
		gameClearScene->Update();
		break; // ★追加
	default:
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kGameClear:
		gameClearScene->Draw();
		break; // ★追加
	default:
		break;
	}
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"LE2C_21_チハラ_シゴウ");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		imgui->Begin();

		ChangeScene();
		UpdateScene();

		imgui->End();

		dxCommon->PreDraw();

		DrawScene();

		AxisIndicator::GetInstance()->Draw();
		PrimitiveDrawer::GetInstance()->Reset();

		// imgui->Draw();

		dxCommon->PostDraw();
	}

	// 後始末
	delete titleScene;
	titleScene = nullptr;
	delete gameScene;
	gameScene = nullptr;
	delete gameOverScene;
	gameOverScene = nullptr;
	delete gameClearScene;
	gameClearScene = nullptr; // ★追加

	KamataEngine::Finalize();
	return 0;
}
