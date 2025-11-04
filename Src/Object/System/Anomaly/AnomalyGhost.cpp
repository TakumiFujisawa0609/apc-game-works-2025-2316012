#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Core/Game/CameraScreen.h"
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
	// 基底クラスの処理
	AnomalyBase::Occurrence(param);

	// ゴーストを出現
	auto ghost = std::make_unique<Ghost>(charaMng_.GetCharacterParam(CharacterManager::TYPE::GHOST));

	// カメラモード状態に合わせて活動状態を設定
	CameraScreen* screen = dynamic_cast<CameraScreen*>(&systemMng_.GetGamsSystem(GameSystemManager::TYPE::SCREEN));
	bool isActive = screen->GetScreenMode() == CameraScreen::SCREEN_MODE::NIGHT_VIEW;		// 暗視状態のみ活動させる
	ghost->SetIsActive(isActive);

	// 読み込み処理
	ghost->Create(param);

	// キャラクター管理に登録
	charaMng_.AddCharacter(CharacterManager::TYPE::GHOST, std::move(ghost));
}