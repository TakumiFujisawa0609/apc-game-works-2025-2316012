#include "../../Object/System/Anomaly/AnomalyBase.h"
#include "../../Object/System/Anomaly/AnomalyAppearance.h"
#include "../../Utility/UtilityLoad.h"
#include "../../Core/Common/Timer.h"
#include "AnomalyManager.h"

void AnomalyManager::Load()
{
	// 異変情報の格納
	anomalyFile_ = UtilityLoad::GetJsonMapData(ANOMALY_FILE);

	// 出現異変処理
	anomalyMap_[TYPE::APPEARANCE] = std::make_unique<AnomalyAppearance>();

	// タイマー
	timer_ = std::make_unique<Timer>(50);
}

void AnomalyManager::Init()
{
	// タイマー初期化
	timer_->Init();

	// 異変発生
	OccurAnomaly(TYPE::APPEARANCE);
}

void AnomalyManager::Update()
{

}

void AnomalyManager::Draw()
{

}

void AnomalyManager::OccurAnomaly(const TYPE type)
{
	const std::string typString = ANOMALY_LIST[static_cast<int>(type)];
	int size = anomalyFile_[typString].size();
	for (int i = 0; i < size; i++)
	{
		// 異変発生
		anomalyMap_[type]->Occurrence(anomalyFile_[typString][i]);
	}
}

AnomalyManager::AnomalyManager()
{
}

AnomalyManager::~AnomalyManager()
{
}
