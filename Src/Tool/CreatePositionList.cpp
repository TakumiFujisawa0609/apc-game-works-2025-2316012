#include <unordered_map>
#include "../Manager/Generic/InputManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Actor/Character/Player.h"
#include "CreatePositionList.h"

// JSON名前空間
using Json = nlohmann::json;

CreatePositionList::CreatePositionList(const std::string& fileName, const Transform& player):
	fileName_(fileName),
	player_(player),
	inputMng_(InputManager::GetInstance())
{
	lastPos_ = VGet(0, 0, 0);
	posList_.clear();
}

CreatePositionList::~CreatePositionList()
{
	Create();
}

void CreatePositionList::Update()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::CREATE_POSITION))
	{
		lastPos_ = player_.pos;
		posList_.emplace_back(lastPos_);
	}
}

void CreatePositionList::Draw()
{
	for (const auto& pos : posList_)
	{
		DrawSphere3D(pos, 10, 16, UtilityCommon::CYAN, UtilityCommon::CYAN, TRUE);
	}
	DrawFormatString(0, 200, UtilityCommon::RED, L"最後に格納した座標:%2f,%2f,%2f", lastPos_.x, lastPos_.y, lastPos_.z);
}

void CreatePositionList::Create()
{
	//JSON配列を生成
	Json jsonArray = Json::array();

	// 座標を格納
	for (const auto& pos : posList_)
	{
		Json jsonObject;
		jsonObject["pos"]["x"] = pos.x;
		jsonObject["pos"]["y"] = pos.y;
		jsonObject["pos"]["z"] = pos.z;

		jsonArray.push_back(jsonObject);
	}

	jsonArray.dump(2);
	std::ofstream outPutFile(fileName_ + ".json");
	if (outPutFile.is_open())
	{
		outPutFile << jsonArray;
		outPutFile.close();
	} 
	// ファイルが開けなかった場合
	else 	
	{
		std::cerr << "エラー: ファイル \"" << fileName_ << "\" を開けませんでした。\n";
	}
}
