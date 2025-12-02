#include "../../Object/Actor/Stage/StageObjectBase.h"
#include "../../Object/Actor/Stage/StageObjectFactory.h"
#include "../../Object/Actor/Stage/StageMain.h"
#include "../../Object/System/Load/ParameterLoad.h"
#include "../../Utility/UtilityLoad.h"
#include "../../Object/Actor/Stage/Sub/Grass.h"
#include "CharacterManager.h"
#include "StageManager.h"

void StageManager::Load()
{
	// パラメータステージマップを取得
	paramStageMap_ = UtilityLoad::GetJsonMapArrayData(STAGE_FILE_NAME);

	// パラメーターコライダーマップを取得
	stageObjectColliserInfoMap_ = UtilityLoad::GetJsonMapArrayData(COLLIDER_FILE_NAME);

	// ファクトリーの生成
	auto factory = std::make_unique<StageObjectFactory>();

	// パラメータ数分オブジェクト生成
	for (auto& params : paramStageMap_)
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
			std::unique_ptr<StageObjectBase> object = factory->Create(params.first, param, collInfo->second[0]);

			// オブジェクト読み込み
			object->Load();

			// 配列に格納
			objects.push_back(std::move(object));
		}
		
		// マップに登録
		stageObjectsMap_.emplace(params.first, std::move(objects));
	}

	for (auto& name : MAIN_STAGES)
	{
		StageObjectBase* object = stageObjectsMap_[name][0].get();
		StageMain* stage = dynamic_cast<StageMain*>(object);
		if (stage != nullptr)
		{
			// キャスト成功
			mainStages_.push_back(stage);
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

	// 初期値
	drawTagList_.push_back("A");
	drawTagList_.push_back("B");
	drawTagList_.push_back("C");
	drawTagList_.push_back("D");
	drawTagList_.push_back("E");
	drawTagList_.push_back("R");
	drawTagList_.push_back("CA");
	drawTagList_.push_back("CB");
	drawTagList_.push_back("CC");

	// 透過リストと通常描画リストを分別
	// 全オブジェクトを巡回し、リストに分類
	for (auto& pair : stageObjectsMap_) 
	{
		for (auto& obj : pair.second)
		{
			if (obj->IsTranslucent())
			{
				translucentList_.push_back(obj.get());
			}
			else 
			{
				opaqueList_.push_back(obj.get());
			}
		}
	}
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

	for (auto& grass : grassList_)
	{
		grass->Update();
	}

	// カメラ範囲か調べる
	CheckMainRoomInClipCameraView();
}

void StageManager::Draw()
{
	for (const auto& obj : opaqueList_)
	{
		for (const auto& tag : drawTagList_)
		{
			// プレイヤーのタグがオブジェクトと一致する場合
			if (tag == obj->GetRoomTag())
			{
				// オブジェクトの描画
				obj->Draw();

				// 次へ
				continue;
			}
		}
	}

	for (const auto& obj : translucentList_)
	{
		for (const auto& tag : drawTagList_)
		{
			// プレイヤーのタグがオブジェクトと一致する場合
			if (tag == obj->GetRoomTag())
			{
				// オブジェクトの描画
				obj->Draw();

				// 次へ
				continue;
			}
		}
	}

	for (auto& grass : grassList_)
	{
		grass->Draw();
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

void StageManager::DeleteGrass()
{
	grassList_.clear();
}

void StageManager::Add(const std::string& type, std::unique_ptr<StageObjectBase> stageObject)
{
	// 描画リスト登録用にポインタを保持
	StageObjectBase* obj = stageObject.get();

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
	}

	// 描画リストに格納
	// 透過がある場合
	if (obj->IsTranslucent())
	{
		translucentList_.push_back(obj);
	}
	// ない場合
	else
	{
		opaqueList_.push_back(obj);
	}
}

void StageManager::AddGrass(std::unique_ptr<Grass> grass)
{
	grassList_.push_back(std::move(grass));
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
	for (auto& stage : mainStages_)
	{
		if (stage->CheckCameraViewClip())
		{
			drawTagList_.push_back(stage->GetRoomTag());
		}
	}
}

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
	mainStages_.clear();
	translucentList_.clear();
	opaqueList_.clear();
}
