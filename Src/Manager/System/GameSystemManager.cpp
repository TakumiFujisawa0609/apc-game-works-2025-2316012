#include "../../Manager/Generic/CharacterManager.h"
#include "../../Object/Actor/Character/Player.h"
#include "../../Core/Game/CoreGameBase.h"
#include "../../Core/Game/ReportSystem.h"
#include "../../Core/Game/GameManual.h"
#include "../../Core/Game/GameTime.h"
#include "../../Core/Game/CameraScreen.h"
#include "GameSystemManager.h"

void GameSystemManager::Load()
{	
	// スクリーンの生成
	auto screen = std::make_unique<CameraScreen>();
	systemsMap_.emplace(TYPE::SCREEN, std::move(screen));
	
	// レポートの生成
	Player& player = dynamic_cast<Player&>(CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER));	// プレイヤークラスの取得
	auto report = std::make_unique<ReportSystem>(player);
	systemsMap_.emplace(TYPE::REPORT, std::move(report));

	// ゲーム時間の生成
	auto gameTime = std::make_unique<GameTime>();
	systemsMap_.emplace(TYPE::GAME_TIMER, std::move(gameTime));

	// マニュアルの生成
	auto manual = std::make_unique<GameManual>();
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
		system.second->Update();
	}
}

void GameSystemManager::Draw()
{
	// 描画処理
	for (auto& system : systemsMap_)
	{
		system.second->Draw();
	}
}

GameSystemManager::GameSystemManager()
{
}

GameSystemManager::~GameSystemManager()
{
}