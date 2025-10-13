#include "../../Object/System/Anomaly/AnomalyBase.h"
#include "../../Utility/UtilityLoad.h"
#include "../../Core/Common/Timer.h"
#include "AnomalyManager.h"

void AnomalyManager::Load()
{
	// 異変情報の格納
	anomalyFile_ = UtilityLoad::GetJsonData(ANOMALY_FILE);

	// タイマー
	timer_ = std::make_unique<Timer>();


}

void AnomalyManager::Init()
{
	// タイマー初期化
	timer_->Init();
}

void AnomalyManager::Update()
{

}

void AnomalyManager::Draw()
{

}

void AnomalyManager::OccurAnomaly()
{
}

AnomalyManager::AnomalyManager()
{
}

AnomalyManager::~AnomalyManager()
{
}
