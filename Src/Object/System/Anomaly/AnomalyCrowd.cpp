#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../Actor/Stage/CrowdRoom.h"
#include "../../Actor/Stage/Sub/Human.h"
#include "AnomalyCrowd.h"

AnomalyCrowd::AnomalyCrowd(const Json& param) :
	AnomalyBase(param),
	MAP_PARAM(param)
{
}

AnomalyCrowd::~AnomalyCrowd()
{
}

void AnomalyCrowd::Occurrence()
{
	// 座標配列のサイズ分回す
	for (const auto& param : MAP_PARAM["posList"])
	{
		// 生成位置を決定
		VECTOR pos = { param["x"], param["y"], param["z"] };

		// 人の生成
		std::unique_ptr<Human> human = std::make_unique<Human>(pos);

		// 初期化
		human->Init();

		// 追加
		stageMng_.AddBillboardObject(StageManager::BILLBOARD_OBJ_TYPE::HUMAN, std::move(human));
	}

	// コライダー情報を取得
	auto& colliderParam = stageMng_.GetStageObjectColliderParam(KEY);	
	
	// 衝突判定用のステージを生成
	std::unique_ptr<CrowdRoom> crowdRoom = std::make_unique<CrowdRoom>(KEY, MAP_PARAM, colliderParam);

	// 異変発生処理
	crowdRoom->SetAnomaly();

	// 効果音の再生
	sndMng_.PlaySe(SoundType::SE::CROWD);

	// 管理クラスに格納
	stageMng_.Add(KEY, std::move(crowdRoom));
}
