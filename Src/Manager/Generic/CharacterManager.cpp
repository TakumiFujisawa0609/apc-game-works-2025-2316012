#include "../../Utility/UtilityLoad.h"
#include "../../Object/Actor/Character/CharacterBase.h"
#include "../../Object/Actor/Character/Player.h"
#include "CharacterManager.h"

void CharacterManager::Load()
{
	// パラメータ読み込み
	paramMap_ = UtilityLoad::GetJsonData(FILE_NAME);

	// プレイヤー生成
	auto player = std::make_unique<Player>(paramMap_[NAME_LIST[static_cast<int>(TYPE::PLAYER)]].front());

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

void CharacterManager::AddCharacter(const TYPE type, const std::unique_ptr<CharacterBase> character)
{
	characterMap_.emplace(type, std::move(character));
}

CharacterManager::CharacterManager()
{
}