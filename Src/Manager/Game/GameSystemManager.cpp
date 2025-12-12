#include <nlohmann/json.hpp>
#include "../../Manager/Game/CharacterManager.h"
#include "../../Object/Actor/Character/Player.h"
#include "../../Core/Game/CoreGameBase.h"
#include "../../Core/Game/ReportSystem.h"
#include "../../Core/Game/GameManual.h"
#include "../../Core/Game/GameTime.h"
#include "../../Core/Game/CameraScreen.h"
#include "../../Core/Game/Message.h"
#include "../../Utility/UtilityLoad.h"
#include "GameSystemManager.h"

// JSON名前空間
using Json = nlohmann::json;

void GameSystemManager::Load()
{	
	// パラメータ読み込み
	auto paramMap = UtilityLoad::GetJsonMapArrayData(FILE_NAME);

	// スクリーンの生成
	auto screen = std::make_unique<CameraScreen>(paramMap[NAME_LIST[static_cast<int>(TYPE::SCREEN)]].front());
	systemsMap_.emplace(TYPE::SCREEN, std::move(screen));
	
	// レポートの生成
	Player& player = dynamic_cast<Player&>(CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER));	// プレイヤークラスの取得
	auto report = std::make_unique<ReportSystem>(paramMap[NAME_LIST[static_cast<int>(TYPE::REPORT)]].front(), player);
	systemsMap_.emplace(TYPE::REPORT, std::move(report));

	// メッセージの生成
	auto message = std::make_unique<Message>(paramMap[NAME_LIST[static_cast<int>(TYPE::MESSAGE)]].front());
	systemsMap_.emplace(TYPE::MESSAGE, std::move(message));

	// ゲーム時間の生成
	auto gameTime = std::make_unique<GameTime>(paramMap[NAME_LIST[static_cast<int>(TYPE::GAME_TIMER)]].front());
	systemsMap_.emplace(TYPE::GAME_TIMER, std::move(gameTime));

	// マニュアルの生成
	auto manual = std::make_unique<GameManual>(paramMap[NAME_LIST[static_cast<int>(TYPE::MANUAL)]].front());
	systemsMap_.emplace(TYPE::MANUAL, std::move(manual));

	// 読み込み
	for (auto& system : systemsMap_)
	{
		system.second->Load();
	}
}

void GameSystemManager::Init()
{
	// 初期化
	for (auto& system : systemsMap_)
	{
		system.second->Init();
	}
}

void GameSystemManager::Update()
{
	// 更新処理
	for (auto& system : systemsMap_)
	{
		if (system.second->IsActive())
		{
			system.second->Update();
		}
	}
}

void GameSystemManager::Draw()
{
	// 描画処理
	for (auto& system : systemsMap_)
	{
		if (system.second->IsActive())
		{
			system.second->Draw();
		}
	}
}

void GameSystemManager::ChangeMessage(const Message::TYPE type)
{
	CoreGameBase* system = systemsMap_.at(TYPE::MESSAGE).get();
	Message* message = dynamic_cast<Message*>(system);

	if (message != nullptr)
	{
		message->ChangeMessage(type);
	}
	
}

void GameSystemManager::SetIsActiveSystem(const TYPE type, const bool isActive)
{
	// 状態変更
	systemsMap_[type]->SetIsActive(isActive);
}

GameSystemManager::GameSystemManager()
{
}

GameSystemManager::~GameSystemManager()
{
}