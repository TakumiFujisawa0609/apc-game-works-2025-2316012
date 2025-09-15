#include "CharacterManager.h"
#include "../../Object/Character/CharacterBase.h"
#include "../../Object/Character/Player.h"

void CharacterManager::Load()
{
	auto player = std::make_unique<Player>();
	player->Load();
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
}
