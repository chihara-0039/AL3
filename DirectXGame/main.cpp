#include "GameScene.h"
#include "KamataEngine.h"
#include "SceneManager.h"
#include <Windows.h>

// これを書くことでKamataEmgine::と書かずに済む
using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_21_チハラ_シゴウ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->ChangeScene(SceneName::TITLE); // 最初はタイトルから

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// 描画開始
		dxCommon->PreDraw();

		// ゲームシーンの更新
		sceneManager->Update();

		// ゲームシーンの描画
		sceneManager->Draw();

		// ここに描画処理を記述する

		// 描画終了
		dxCommon->PostDraw();
	}

	// 解放処理
	sceneManager->Finalize();
	

	// エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}