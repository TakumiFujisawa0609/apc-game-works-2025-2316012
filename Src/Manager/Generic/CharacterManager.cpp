#include "CharacterManager.h"
#include "../../Object/Character/CharacterBase.h"
#include "../../Object/Character/Player.h"
#include "../../Object/Character/Parameter/ParameterBase.h"
#include "../../Object/Character/Parameter/ParameterPlayer.h"
#include "../../Object/System/Load/ParameterLoadCharacter.h"

void CharacterManager::Load()
{
	//パラメータ読み込み
	paramLoad_ = std::make_unique<ParameterLoadCharacter>();
	paramLoad_->Load();

	//プレイヤーパラメータを取得
	ParameterPlayer* playerParam = nullptr;
	ParameterBase* baseParam = paramLoad_->GetParameter("player");
	if (baseParam != nullptr)
	{
		// ダウンキャスト（dynamic_castを使用するのが安全）
		playerParam = dynamic_cast<ParameterPlayer*>(baseParam);
	}

	//プレイヤー生成
	auto player = std::make_unique<Player>(*playerParam);

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
