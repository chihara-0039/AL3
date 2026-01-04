#include "SceneManager.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::ChangeScene(SceneName next) {

	// 旧シーンの終了
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}

	currentName_ = next;

	// 新シーンを生成
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
}

void SceneManager::Update() {
	if (currentScene_) {
		currentScene_->Update();
	}
}

void SceneManager::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}
}

void SceneManager::Finalize() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
}
