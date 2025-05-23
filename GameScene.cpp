#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;


// 初期化
void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	textureHandle = TextureManager::Load("uvChecker.png");

	
	// 3Dモデルデータの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// カメラの初期化
	camera_.Initialize();


	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle, &camera_);

	// 3Dモデルデータの生成(block)AL3_02_02
	modelBlock_ = Model::Create();


	//天球の作成
	skydome_ = new Skydome();

	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &camera_);


	//マップチップの初期化
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");



	// 初期化
	// 要素数
	const uint32_t kNumBlockVirtial = 10;
	const uint32_t kNumBlockHorizontal = 20;
	
	// ブロック１個文の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	
	// 要素数を変更する
	// 列数の設定(縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtial);
	for (uint32_t i = 0; i < kNumBlockVirtial; ++i) {
		// 一列の要素数を設定(縦方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtial; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i + j) % 2 == 0) {
				continue;
			}
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	void GameScene::GenerateBlocks_() {

		uint32_t numBlockVirtical = mapChipField_->kNumBlockVirtical();
		uint32_t numBlockHorizontal = mapChipField_->kNumBlockHorizontal();

		worldTransformBlocks_.resize(numBlockVirtical);
		for (uint32_t i = 0; i < numBlockVirtical; ++i) {
			worldTransformBlocks_[i].resize(numBlockHorizontal);
		}

		// ブロックの生成
		for (uint32_t i = 0; i < numBlockVirtical; ++i) {

			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

				if (mapChipField_->GetMapChipPositionByIndex(j, i) == MapChipType::kBlock) {
					WorldTransform* worldTransform = new WorldTransform();
					worldTransform->Initialize();
					worldTransformBlocks_[i][j] = worldTransform;
					worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				}
			}
		}
	}


	
}

// 更新
void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	// ブロックの更新
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
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
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送

		camera_.UpdateMatrix();
	}
}

// 描画
void GameScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 自キャラの描画
	model_->PreDraw(dxCommon->GetCommandList());
	player_->Draw();
	// ブロックの描画
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	skydome_->Draw();

	model_->PostDraw();
}


// デストラクタ
GameScene::~GameScene() {
	// 3Dモデルデータの解散
	delete model_;

	// 自キャラの解散
	delete player_;

	/* 3Dモデルデータの解放(block)AL3_02_02*/
	for (std::vector<WorldTransform*>& worldTransformBkockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBkockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	//天球の解放
	delete modelSkydome_;

	// デバッグカメラの解放
	delete debugCamera_;

	//マップチップフィールドの解放
	delete mapChipField_;
}



