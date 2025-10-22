#include <DxLib.h>
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
	timer_ = std::make_unique<Timer>(FIRST_TIME);
}

void AnomalyManager::Init()
{
	// タイマー初期化
	timer_->InitCountUp();
}

void AnomalyManager::Update()
{
	// 制限時間に達したとき
	if (timer_->CountUp())
	{
		// 異変ファイルが空の場合
		if (anomalyFile_.empty())
		{
			// 処理を飛ばす
			return;
		}

		// 異変発生
		OccurAnomaly(TYPE::APPEARANCE);

		// 次回までの時間をランダム設定
		timer_->SetGoalTime(ANOMALY_TIME_MIN + GetRand(ANOMALY_TIME_MAX - ANOMALY_TIME_MIN));

		// タイマー初期化
		timer_->InitCountUp();
	}
}

void AnomalyManager::DebugDraw()
{
	DrawFormatString(0, 400, 0xff0000, L"発生までの時間：%d", timer_->GetCount());
}

void AnomalyManager::OccurAnomaly(const TYPE type)
{
	// 異変の種類のストリングを取得
	const std::string typeString = ANOMALY_LIST[static_cast<int>(type)];

	// サイズ定義
	const int size = static_cast<int>(anomalyFile_[typeString].size());
	
	// ランダムで配列番号を取得
	int index = GetRand(size - 1);

	// 異変発生
	anomalyMap_[type]->Occurrence(anomalyFile_[typeString][index]);

	// 異変のファイルからその要素を削除
	if (anomalyFile_.is_array() && anomalyFile_.size() > index)
	{
		anomalyFile_.erase(anomalyFile_.begin() + index);
	}
}

AnomalyManager::AnomalyManager()
{
}

AnomalyManager::~AnomalyManager()
{
}