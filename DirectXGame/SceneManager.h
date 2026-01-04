#pragma once
#include <memory>

/// シーンの種類
enum class SceneName {
	TITLE,
	GAME,
	CLEAR,
	GAMEOVER,
};

/// シーン共通インターフェース
class IScene {
public:
	virtual ~IScene() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};

/// シーン管理クラス（シングルトン）
class SceneManager {
public:
	static SceneManager* GetInstance();

	// シーン切り替え
	void ChangeScene(SceneName next);

	// 現在のシーンの更新・描画
	void Update();
	void Draw();

	// 終了処理（アプリ終了時に1回呼ぶ）
	void Finalize();

	SceneName GetCurrentSceneName() const { return currentName_; }

private:
	SceneManager() = default;
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

private:
	std::unique_ptr<IScene> currentScene_;
	SceneName currentName_ = SceneName::TITLE;
};
