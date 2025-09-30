#include "../../Object/Actor/Character/CharacterBase.h"
#include "../../Object/Actor/Character/Player.h"
#include "../../Object/System/Load/ParameterLoad.h"
#include "CharacterManager.h"

void CharacterManager::Load()
{
	// パラメータ読み込み
	paramLoad_ = std::make_unique<ParameterLoad>(FILE_NAME);
	paramLoad_->Load();

	// プレイヤー生成
	auto player = std::make_unique<Player>(paramLoad_->GetParameterFile(NAME_LIST[static_cast<int>(TYPE::PLAYER)]).front());

	// プレイヤー読み込み
	player->Load();

	// マップに登録
	characterMap_.emplace(TYPE::PLAYER, std::move(player));
}

void CharacterManager::Init()
{
	for (auto& character : characterMap_)
	{
		character.second->Init();
	}
}

void CharacterManager::Update()
{
	for (auto& character : characterMap_)
	{
		character.second->Update();
	}
}

void CharacterManager::Draw()
{
	for (auto& character : characterMap_)
	{
		character.second->Draw();
	}
}

CharacterManager::CharacterManager()
{
	paramLoad_ = nullptr;
}