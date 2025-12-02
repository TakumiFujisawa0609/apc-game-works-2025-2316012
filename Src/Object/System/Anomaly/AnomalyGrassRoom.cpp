#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../Actor/Stage/GrassRoom.h"
#include "AnomalyGrassRoom.h"

AnomalyGrassRoom::AnomalyGrassRoom(const Json& param) :
	AnomalyBase(param)
{
}

AnomalyGrassRoom::~AnomalyGrassRoom()
{
}

void AnomalyGrassRoom::Occurrence()
{
	// 基底クラスの処理
	AnomalyBase::Occurrence();

	// ステージのポインタ
	GrassRoom* stage = dynamic_cast<GrassRoom*>(stageMng_.GetStageObjects(KEY)[0].get());

	// 異変設定
	stage->SetAnomaly();

	// 効果音の配列
	std::vector<SoundType::SE> seTypes = { SoundType::SE::NOISE_GATAN, SoundType::SE::NOISE_PACHI,SoundType::SE::NOISE_SWITCH,SoundType::SE::NOISE_GON, SoundType::SE::NOISE_METAL, SoundType::SE::GIRLS_HELP, SoundType::SE::GIRLS_LAUGHTER };

	// ランダムで番号を取得
	int seIndex = GetRand(static_cast<int>(seTypes.size() - 1));

	// ランダムで音量を設定
	int volume = 30 + GetRand(20);

	// 効果音再生
	sndMng_.PlaySe(seTypes[seIndex], volume);
}