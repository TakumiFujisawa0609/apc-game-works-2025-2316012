#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Resource/SoundManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../Actor/Stage/StageMain.h"
#include "AnomalyBloodyRoom.h"

AnomalyBloodyRoom::AnomalyBloodyRoom()
{
}

AnomalyBloodyRoom::~AnomalyBloodyRoom()
{
}

void AnomalyBloodyRoom::Load()
{
}

void AnomalyBloodyRoom::Init()
{
}

void AnomalyBloodyRoom::Occurrence(Json& param)
{
	// 基底クラスの処理
	AnomalyBase::Occurrence(param);

	// ランダム
	int index = GetRand(OBJS);

	// キーを値別に取得
	std::string key = index == 0 ? "RoomB" : "RoomC";

	// ステージのポインタ
	StageMain* stage = dynamic_cast<StageMain*>(stageMng_.GetStageObjects(key)[0].get());

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
