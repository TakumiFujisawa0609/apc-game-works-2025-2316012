#include "../../Object/Actor/Stage/StageObjectBase.h"
#include "../../Object/Actor/Stage/StageObjectFactory.h"
#include "../../Object/Actor/Stage/Room.h"
#include "../../Object/System/Load/ParameterLoad.h"
#include "../../Utility/UtilityLoad.h"
#include "CharacterManager.h"
#include "StageManager.h"

void StageManager::Load()
{
	// パラメータステージマップを取得
	auto& paramStageMap = UtilityLoad::GetJsonMapArrayData(STAGE_FILE_NAME);

	// パラメーターコライダーマップを取得
	stageObjectColliserInfoMap_ = UtilityLoad::GetJsonMapArrayData(COLLIDER_FILE_NAME);

	// ファクトリーの生成
	auto factory = std::make_unique<StageObjectFactory>();

	// パラメータ数分オブジェクト生成
	for (auto& params : paramStageMap)
	{		
		// オブジェクト格納用配列
		std::vector<std::unique_ptr<StageObjectBase>> objects;

		// 要素分パラメータ格納
		for (auto& param : params.second)
		{
			// コライダー情報を検索
			const auto& collInfo = stageObjectColliserInfoMap_.find(params.first);

			// 中身が空もしくは配列で格納されている場合
			if (collInfo->second.empty())
			{
				// 想定外のため終了
				return;
			}

			// オブジェクト生成
			auto object = factory->Create(params.first, param, collInfo->second[0]);

			// オブジェクト読み込み
			object->Load();

			// 配列に格納
			objects.push_back(std::move(object));
		}
		
		// マップに登録
		stageObjectsMap_.emplace(params.first, std::move(objects));
	}

	for (auto& roomName : ROOMS)
	{
		StageObjectBase* object = stageObjectsMap_[roomName][0].get();
		Room* room = dynamic_cast<Room*>(object);
		if (room != nullptr) 
		{
			// キャスト成功
			rooms_.push_back(room);
		}
	}
}

void StageManager::Init()
{
	for (auto& objects : stageObjectsMap_)
	{
		for (auto& object : objects.second)
		{
			object->Init();
		}
	}

	drawTagList_.push_back("A");
}

void StageManager::Update()
{
	for (auto& objects : stageObjectsMap_)
	{
		for (auto& object : objects.second)
		{
			object->Update();
		}
	}

	// カメラ範囲か調べる
	CheckMainRoomInClipCameraView();
}

void StageManager::Draw()
{
	for (auto& objects : stageObjectsMap_)
	{
		for (auto& object : objects.second)
		{
			for (const auto& tag : drawTagList_)
			{
				// プレイヤーのタグがオブジェクトと一致する場合
				if (tag == object->GetRoomTag())
				{
					// オブジェクトの描画
					object->Draw();

					// 次へ
					continue;
				}
			}
		}
	}
}

void StageManager::Sweep()
{	
	// マップ全体をイテレート
	for (auto map = stageObjectsMap_.begin(); map != stageObjectsMap_.end();)
	{
		// キャラクターリストを取得
		auto& objects = map->second;

		// キャラクターの並び替え
		auto it = std::remove_if(
			objects.begin(),
			objects.end(),
			[](const std::unique_ptr<StageObjectBase>& object)
			{
				// 削除判定
				return object->IsDelete();
			}
		);

		// 範囲内のキャラで削除予定ならマップから削除
		objects.erase(it, objects.end());

		// キャラクターリストが空の場合
		if (objects.empty())
		{
			// マップからも削除
			map = stageObjectsMap_.erase(map);
		}
		// 空でない場合
		else
		{
			// 次の要素に進む
			++map;
		}
	}
}

void StageManager::Add(const std::string& type, std::unique_ptr<StageObjectBase> stageObject)
{
	// 指定されたTYPEのキーがマップに存在するか検索
	auto it = stageObjectsMap_.find(type);
	if (it != stageObjectsMap_.end())
	{
		// 存在する場合はそのまま追加
		it->second.push_back(std::move(stageObject));
	}
	else
	{
		// 存在しない場合は新たにキーを作成して追加
		std::vector<std::unique_ptr<StageObjectBase>> objects;
		objects.push_back(std::move(stageObject));
		stageObjectsMap_.emplace(type, std::move(objects));
		return;
	}
}

const Json& StageManager::GetStageObjectColliderParam(const std::string& key) const
{
	// マップから指定されたキーの要素を検索
	auto it = stageObjectColliserInfoMap_.find(key);

	// 要素が見つからない場合
	if (it == stageObjectColliserInfoMap_.end())
	{
		// 空のJsonを返す
		static const Json emptyJson = Json();
		return emptyJson; 
	}

	// 情報を返す
	return it->second.front();
}

void StageManager::CheckMainRoomInClipCameraView()
{
	drawTagList_.clear();
	for (auto& room : rooms_)
	{
		if (room->CheckCameraViewClip())
		{
			drawTagList_.push_back(room->GetRoomTag());
		}
	}
}

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
	rooms_.clear();
}
