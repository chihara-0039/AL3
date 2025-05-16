#include "GameScene.h"

using namespace KamataEngine;
// 初期化
void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	textureHandle = TextureManager::Load("uvChecker.png");

	
	// 3Dモデルデータの生成
	model = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// デバッグカメラの生成
	debugCamera = new DebugCamera(1280, 720);

	// カメラの初期化
	camera.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model, textureHandle, &camera);

	// 3Dモデルデータの生成(block)AL3_02_02
	modelBlock = Model::Create();

	//天球の作成
	skydome_ = new Skydome();

	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &camera);

	// 初期化
	// 要素数
	const uint32_t kNumBlockVirtial = 10;
	const uint32_t kNumBlockHorizontal = 20;
	
	// ブロック１個文の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	
	// 要素数を変更する
	// 列数の設定(縦方向のブロック数)
	worldTransformBlocks.resize(kNumBlockVirtial);
	for (uint32_t i = 0; i < kNumBlockVirtial; ++i) {
		// 一列の要素数を設定(縦方向のブロック数)
		worldTransformBlocks[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtial; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i + j) % 2 == 0) {
				continue;
			}
			worldTransformBlocks[i][j] = new WorldTransform();
			worldTransformBlocks[i][j]->Initialize();
			worldTransformBlocks[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	
}

// 更新
void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	// ブロックの更新
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = math_.MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}

	//天球の更新
	skydome_->Update();

#ifdef _DEBUG
	// デバックの時Cキーを押すと状態が反転する
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
#endif // ! _DEBUG
	// カメラの処理
	if (isDebugCameraActive) {
		// デバッグカメラの更新
		debugCamera->Update();
		camera.matView = debugCamera->GetCamera().matView;
		camera.matProjection = debugCamera->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送

		camera.UpdateMatrix();
	}
}

// 描画
void GameScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 自キャラの描画
	model->PreDraw(dxCommon->GetCommandList());
	player_->Draw();
	// ブロックの描画
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock->Draw(*worldTransformBlock, camera);
		}
	}

	skydome_->Draw();

	model->PostDraw();
}


// デストラクタ
GameScene::~GameScene() {
	// 3Dモデルデータの解散
	delete model;

	// 自キャラの解散
	delete player_;

	/* 3Dモデルデータの解放(block)AL3_02_02*/
	for (std::vector<WorldTransform*>& worldTransformBkockLine : worldTransformBlocks) {
		for (WorldTransform* worldTransformBlock : worldTransformBkockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks.clear();

	//天球の解放
	delete modelSkydome_;

	// デバッグカメラの解放
	delete debugCamera;
}



