#include "../../../Manager/Generic/CharacterManager.h"
#include "../../Actor/Character/Ghost.h"
#include "AnomalyAppearance.h"

AnomalyAppearance::AnomalyAppearance()
{
}

AnomalyAppearance::~AnomalyAppearance()
{
}

void AnomalyAppearance::Init()
{
}

void AnomalyAppearance::Update()
{
}

void AnomalyAppearance::Draw()
{
}

void AnomalyAppearance::Occurrence(Json& param)
{
	// ゴーストを出現
	auto ghost = std::make_unique<Ghost>(charaMng_.GetCharacterParam(CharacterManager::TYPE::GHOST));
	
	// 読み込み処理
	ghost->Create();

	// 初期化処理
	ghost->Init();
}

void AnomalyAppearance::Delete()
{
}
