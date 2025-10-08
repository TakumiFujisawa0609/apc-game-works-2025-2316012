#include "../../Manager/Generic/CharacterManager.h"
#include "../../Object/Actor/Character/Player.h"
#include "../../Core/CoreBase.h"
#include "../../Core/Game/Report/ReportSystem.h"
#include "GameSystemManager.h"

void GameSystemManager::Load()
{
	// プレイヤークラスの取得
	Player& player = dynamic_cast<Player&>(CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER));

	// レポートの生成
	auto report = std::make_unique<ReportSystem>(player);
	systemsMap_.emplace(TYPE::REPORT, std::move(report));

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