#include <nlohmann/json.hpp>
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../Actor/Stage/ChairMountain.h"
#include "AnomalyChairMountain.h"

AnomalyChairMountain::AnomalyChairMountain(const Json& param) :
	INIT_PARAM(param),
	AnomalyBase(param)
{
}

AnomalyChairMountain::~AnomalyChairMountain()
{
}

void AnomalyChairMountain::Occurrence()
{
	// 基底クラスの処理
	AnomalyBase::Occurrence();

	// 生成
	auto object = std::make_unique<ChairMountain>(KEY, INIT_PARAM, stageMng_.GetStageObjectColliderParam(KEY));

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
