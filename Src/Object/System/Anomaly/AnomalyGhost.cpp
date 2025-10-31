#include "../../../Manager/Generic/CharacterManager.h"
#include "../../Actor/Character/CharacterBase.h"
#include "../../Actor/Character/Ghost.h"
#include "AnomalyGhost.h"

AnomalyGhost::AnomalyGhost()
{
}

AnomalyGhost::~AnomalyGhost()
{
}

void AnomalyGhost::Init()
{
}

void AnomalyGhost::Occurrence(Json& param)
{
	// ゴーストを出現
	auto ghost = std::make_unique<Ghost>(charaMng_.GetCharacterParam(CharacterManager::TYPE::GHOST));

	// 読み込み処理
	ghost->Create(param);

	// キャラクター管理に登録
	charaMng_.AddCharacter(CharacterManager::TYPE::GHOST, std::move(ghost));
}