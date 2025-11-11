#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Resource/SoundManager.h"
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

	// 効果音の配列
	std::vector<SoundType::SE> seTypes = { SoundType::SE::NOISE_GATAN, SoundType::SE::NOISE_PACHI,SoundType::SE::NOISE_SWITCH,SoundType::SE::NOISE_GON, SoundType::SE::NOISE_METAL, SoundType::SE::GIRLS_HELP, SoundType::SE::GIRLS_LAUGHTER };

	// ランダムで番号を取得
	int seIndex = GetRand(static_cast<int>(seTypes.size() - 1));

	// ランダムで音量を設定
	int volume = 30 + GetRand(20);

	// 効果音再生
	sndMng_.PlaySe(seTypes[seIndex], volume);
}