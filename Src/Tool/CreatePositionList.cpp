#include <nlohmann/json.hpp>
#include <unordered_map>
#include "../Manager/Generic/InputManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityLoad.h"
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
	// ファイルの生成
	Create();
}

void CreatePositionList::Update()
{
	// ボタンを押した場合
	if (inputMng_.IsTrgDown(InputManager::TYPE::CREATE_POSITION))
	{	
		// 要素を消していくためイテレーターfor文
		for (auto it = posList_.begin(); it != posList_.end(); )
		{
			// 範囲内にポイントがある場合
			if (Utility3D::Distance(*it, player_.pos) < 20.0f)
			{
				// 削除
				it = posList_.erase(it);
				return;
			}
			else
			{
				// 次の要素へ
				++it;
			}
		}

		// プレイヤー現在地にポイントを生成
		lastPos_ = player_.pos;
		posList_.emplace_back(lastPos_);
	}
	// ファイルを開く
	if (inputMng_.IsTrgDown(InputManager::TYPE::OPEN_FILE))
	{
		// マウス表示
		SetMouseDispFlag(TRUE);

		// ファイルの取得
		std::string file = UtilityLoad::OpenFileDialog();

		// ファイルが空の場合
		if (file.empty())
		{
			return;
			SetMouseDispFlag(false);
		}

		std::ifstream inFile(file);
		if (!inFile.is_open())
		{
			std::cerr << "ファイルを開けませんでした: " << file << "\n";
			return;
			SetMouseDispFlag(false);
		}

		// JSONデータの読み込み
		Json json;
		inFile >> json;

		// リストを初期化
		posList_.clear();

		// 格納
		for (auto& it : json)
		{
			VECTOR pos = { it["pos"]["x"], it["pos"]["y"], it["pos"]["z"] };
			posList_.emplace_back(pos);
		}
		SetMouseDispFlag(false);
	}
}

void CreatePositionList::Draw()
{
	for (const auto& pos : posList_)
	{
		DrawSphere3D(pos, 10, 16, UtilityCommon::LIME, UtilityCommon::LIME, TRUE);
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