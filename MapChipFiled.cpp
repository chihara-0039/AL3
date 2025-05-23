#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <map>
#include "MapChipFiled.h"
#include "KamataEngine.h"

using namespace KamataEngine;


//内部リンケージ
namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

// マップチップデータをリセット
void MapChipField::ResetMapChipData() {

	//  マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDateLine : mapChipData_.data) {
		mapChipDateLine.resize(kNumBlockHorizontal);
	}
}


// マップチップCSVを読み込む
void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	
	// マップチップデータをリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file(filePath);
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;

	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	// CSV内容をパースして mapChipData_ に反映する処理をここに追加
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);
		

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex)
{ 
	return KamataEngine::Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); 
}

MapChipType MapChipField::GetMapChiptypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

