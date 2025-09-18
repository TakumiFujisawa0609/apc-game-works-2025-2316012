#include <nlohmann/json.hpp>
#include "CharacterManager.h"
#include "../../Object/Character/CharacterBase.h"
#include "../../Object/Character/Player.h"
#include "../../Object/System/Load/ParameterLoadCharacter.h"

// JSON名前空間
using Json = nlohmann::json;

void CharacterManager::Load()
{
	//パラメータ読み込み
	paramLoad_ = std::make_unique<ParameterLoadCharacter>();
	paramLoad_->Load();

	//プレイヤー生成
	auto player = std::make_unique<Player>(paramLoad_->GetParameterFile("player"));

	//プレイヤー読み込み
	player->Load();

	//マップに登録
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
