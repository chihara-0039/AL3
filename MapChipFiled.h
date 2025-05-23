#pragma once
#include "KamataEngine.h"

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

enum class MapChipType {
	kBlank,	//空白
	kBlock,	//ブロック
};

class MapChipField {
public:
	// メンバ変数
	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex)
	{ return KamataEngine::Vector3(kBlockWidth * xIndex,
								   kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);

	}

private:

	MapChipType GetMapChiptypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	}
	
	
};
