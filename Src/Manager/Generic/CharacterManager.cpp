#include "../../Utility/UtilityLoad.h"
#include "../../Object/Actor/Character/CharacterBase.h"
#include "../../Object/Actor/Character/Player.h"
#include "../../Object/Actor/Character/Enemy.h"
#include "../../Object/Actor/Character/Ghost.h"
#include "../../Object/Controller/ControllerLight.h"
#include "CharacterManager.h"

void CharacterManager::Load()
{
	// パラメータ読み込み
	paramMap_ = UtilityLoad::GetJsonMapArrayData(FILE_NAME);

	// プレイヤー生成
	std::unique_ptr<Player> player = std::make_unique<Player>(paramMap_[NAME_LIST[static_cast<int>(TYPE::PLAYER)]].front());
	player->Load();	// プレイヤー読み込み
	characterMap_[TYPE::PLAYER].push_back(std::move(player));

	// 敵の生成
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(paramMap_[NAME_LIST[static_cast<int>(TYPE::ENEMY)]].front());
	enemy->Load();	// 敵読み込み
	characterMap_[TYPE::ENEMY].push_back(std::move(enemy));
}

void CharacterManager::Init()
{
	for (auto& characters : characterMap_)
	{
		for (auto& character : characters.second)
		{
			character->Init();
		}
	}
}

void CharacterManager::Update()
{	
	for (auto& characters : characterMap_)
	{
		for (auto& character : characters.second)
		{
			character->Update();
		}
	}
}

void CharacterManager::Draw()
{
	for (auto& characters : characterMap_)
	{
		for (auto& character : characters.second)
		{
			character->Draw();
		}
	}
}

void CharacterManager::Sweep()
{
	// マップ全体をイテレート
	for (auto map = characterMap_.begin(); map != characterMap_.end();)
	{
		// キャラクターリストを取得
		auto& characters = map->second;

		// キャラクターの並び替え
		auto it = std::remove_if(
			characters.begin(),
			characters.end(),
			[](const std::unique_ptr<CharacterBase>& character)
			{
				// 削除判定
				return character->IsDelete();
			}
		);

		// 範囲内のキャラで削除予定ならマップから削除
		characters.erase(it, characters.end());

		// キャラクターリストが空の場合
		if (characters.empty())
		{
			// マップからも削除
			map = characterMap_.erase(map);
		}
		// 空でない場合
		else
		{
			// 次の要素に進む
			++map;
		}
	}
}

void CharacterManager::AddCharacter(const TYPE type, std::unique_ptr<CharacterBase> character)
{
	// 指定されたTYPEのキーがマップに存在するか検索
	auto it = characterMap_.find(type);
	if (it != characterMap_.end())
	{
		// 存在する場合はそのまま追加
		it->second.push_back(std::move(character));
	}
	else
	{
		// 存在しない場合は新たにキーを作成して追加
		std::vector<std::unique_ptr<CharacterBase>> characters;
		characters.push_back(std::move(character));
		characterMap_.emplace(type, std::move(characters));
		return;
	}
}

const VECTOR& CharacterManager::GetPlayerLightPos() const
{
	CharacterBase* character = characterMap_.at(TYPE::PLAYER).front().get();
	Player* player = dynamic_cast<Player*>(character);
	return player->GetControllerLight().GetLightPos();
}

const bool CharacterManager::IsPlayerLight() const
{
	CharacterBase* character = characterMap_.at(TYPE::PLAYER).front().get();
	Player* player = dynamic_cast<Player*>(character);
	return player->GetControllerLight().IsLight();
}

CharacterManager::CharacterManager()
{
}

CharacterManager::~CharacterManager()
{
}
