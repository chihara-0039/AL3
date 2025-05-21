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

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
		return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);

	}

private:

	//メンバ変数
	MapChipData mapChipData_;

	
	MapChipType GetMapChiptypeByIndex(uint32_t xIndex, uint32_t yIndex) {
		if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
			return MapChipType::kBlank;
		}

		if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
			return MapChipType::kBlank;
		}

		return mapChipData_.data[yIndex][xIndex];
	}
	
};
