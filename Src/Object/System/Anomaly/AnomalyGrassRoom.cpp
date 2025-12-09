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

	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::GRASS_LAND, VOLUME);
}