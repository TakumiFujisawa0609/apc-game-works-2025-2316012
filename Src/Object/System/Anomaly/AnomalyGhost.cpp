#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Core/Game/CameraScreen.h"
#include "../../Actor/Character/CharacterBase.h"
#include "../../Actor/Character/Ghost.h"
#include "AnomalyGhost.h"

AnomalyGhost::AnomalyGhost(const Json& param) :
	paramFile_(param),
	AnomalyBase(param)
{
}

AnomalyGhost::~AnomalyGhost()
{
}

void AnomalyGhost::Occurrence()
{
	// 基底クラスの処理
	AnomalyBase::Occurrence();

	// ゴーストを出現
	auto ghost = std::make_unique<Ghost>(charaMng_.GetCharacterParam(CharacterManager::TYPE::GHOST));

	// カメラモード状態に合わせて活動状態を設定
	CameraScreen* screen = dynamic_cast<CameraScreen*>(&systemMng_.GetGamsSystem(GameSystemManager::TYPE::SCREEN));
	bool isActive = screen->GetScreenMode() == CameraScreen::SCREEN_MODE::NIGHT_VIEW;		// 暗視状態のみ活動させる
	ghost->SetIsActive(isActive);

	// サイズ定義
	const int size = static_cast<int>(paramFile_["transforms"].size());

	// ランダムで配列番号を取得
	int index = GetRand(size - 1);

	// 読み込み処理
	ghost->Create(paramFile_["transforms"][index]);

	// 異変のファイルからその要素を削除
	if (paramFile_.is_array() && paramFile_.size() > index)
	{
		paramFile_.erase(paramFile_.begin() + index);
	}

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