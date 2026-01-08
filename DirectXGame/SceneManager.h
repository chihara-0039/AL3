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
	virtual void Finalize() = 0;

	// ★追加：デフォルトは空（シーンが2D/3D不要でもリンクエラーにならない）
	virtual void Draw3D() {}
	virtual void Draw2D() {}

	// ★互換：古い呼び出しが残ってても動く
	virtual void Draw() {
		Draw3D();
		Draw2D();
	}
};

/// シーン管理クラス（シングルトン）
class SceneManager {
public:
	static SceneManager* GetInstance();

	// 最初のシーン生成（任意：main側で1回だけ呼ぶ）
	void Initialize(SceneName start = SceneName::TITLE);

	// シーン切り替え要求（※即時切替はしない。Updateの最後で安全に切り替える）
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

	// ここだけが「即時切替」を行う（SceneのUpdate中に呼ばないこと）
	void ApplySceneChange(SceneName next);

	// スペースを押しっぱなしで次シーンに入ったとき、離すまでUpdateを止める
	bool waitSpaceRelease_ = false;

	// 次シーン要求（Update後に適用）
	bool hasPendingChange_ = false;
	SceneName pendingName_ = SceneName::TITLE;

private:
	std::unique_ptr<IScene> currentScene_;
	SceneName currentName_ = SceneName::TITLE;

	bool isInitialized_ = false;
};
