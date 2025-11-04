#include "../../../Manager/Resource/ResourceManager.h"
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
	int index = GetRand(2);

	// キーを値別に取得
	std::string key = index == 0 ? "RoomB" : "RoomC";

	// ステージのポインタ
	StageMain* stage = dynamic_cast<StageMain*>(stageMng_.GetStageObjects(key)[0].get());

	// 異変設定
	stage->SetAnomaly();

}
