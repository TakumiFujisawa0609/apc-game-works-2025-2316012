#include <nlohmann/json.hpp>
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Resource/SoundManager.h"
#include "../../Actor/Stage/ChairMountain.h"
#include "AnomalyChairMountain.h"

AnomalyChairMountain::AnomalyChairMountain()
{
}

AnomalyChairMountain::~AnomalyChairMountain()
{
}

void AnomalyChairMountain::Load()
{
}

void AnomalyChairMountain::Init()
{
}

void AnomalyChairMountain::Occurrence(Json& param)
{
	// 基底クラスの処理
	AnomalyBase::Occurrence(param);

	// 生成
	auto object = std::make_unique<ChairMountain>(KEY, param, stageMng_.GetStageObjectColliderParam(KEY));

	// 追加
	stageMng_.Add(KEY, std::move(object));

	// 効果音の配列
	std::vector<SoundType::SE> seTypes = { SoundType::SE::NOISE_GATAN, SoundType::SE::NOISE_PACHI,SoundType::SE::NOISE_SWITCH,SoundType::SE::NOISE_GON, SoundType::SE::NOISE_METAL };

	// ランダムで番号を取得
	int seIndex = GetRand(static_cast<int>(seTypes.size() - 1));
	
	// ランダムで音量を設定
	int volume = 30 + GetRand(20);

	// 効果音再生
	sndMng_.PlaySe(seTypes[seIndex], volume);
}
