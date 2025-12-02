#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../Actor/Character/CharacterBase.h"
#include "AnomalyAppearance.h"

AnomalyAppearance::AnomalyAppearance(const Json& param) :
	AnomalyBase(param)
{
}

AnomalyAppearance::~AnomalyAppearance()
{
}

void AnomalyAppearance::Occurrence()
{
	// 基底クラスの処理
	AnomalyBase::Occurrence();

	// 効果音の配列
	std::vector<SoundType::SE> seTypes = { SoundType::SE::NOISE_GATAN, SoundType::SE::NOISE_PACHI,SoundType::SE::NOISE_SWITCH,SoundType::SE::NOISE_GON, SoundType::SE::NOISE_METAL };

	// ランダムで番号を取得
	int seIndex = GetRand(static_cast<int>(seTypes.size() - 1));

	// ランダムで音量を設定
	int volume = 30 + GetRand(20);

	// 効果音再生
	sndMng_.PlaySe(seTypes[seIndex], volume);
}