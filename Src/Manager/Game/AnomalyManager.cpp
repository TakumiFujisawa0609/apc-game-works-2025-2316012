#include <DxLib.h>
#include "../../Object/System/Anomaly/AnomalyBase.h"
#include "../../Object/System/Anomaly/AnomalyAppearance.h"
#include "../../Object/System/Anomaly/AnomalyGhost.h"
#include "../../Object/System/Anomaly/AnomalyPainting.h"
#include "../../Object/System/Anomaly/AnomalyChairMountain.h"
#include "../../Object/System/Anomaly/AnomalyBloodyRoom.h"
#include "../../Object/System/Anomaly/AnomalyGrassRoom.h"
#include "../../Object/System/Anomaly/AnomalyReverseFall.h"
#include "../../Utility/UtilityLoad.h"
#include "../../Core/Common/Timer.h"
#include "AnomalyManager.h"

void AnomalyManager::Load()
{
	// 異変情報の格納
	anomalyFile_ = UtilityLoad::GetJsonMapArrayData(ANOMALY_FILE);

	// 出現異変処理
	anomalyMap_[TYPE::GHOST] = std::make_unique<AnomalyGhost>(anomalyFile_["Ghost"].front());
	anomalyMap_[TYPE::PAINTING] = std::make_unique<AnomalyPainting>(anomalyFile_["Painting"].front());
	anomalyMap_[TYPE::CHAIR_MOUNTAIN] = std::make_unique<AnomalyChairMountain>(anomalyFile_["ChairMountain"].front());
	anomalyMap_[TYPE::BLOODY_ROOM] = std::make_unique<AnomalyBloodyRoom>(anomalyFile_["BloodyRoom"].front());
	anomalyMap_[TYPE::GRASS_ROOM] = std::make_unique<AnomalyGrassRoom>(anomalyFile_["GrassRoom"].front());
	anomalyMap_[TYPE::REVERSE_FALL] = std::make_unique<AnomalyReverseFall>(anomalyFile_["ReverseFall"].front());

	// 各種異変の読み込み処理
	auto& paramFile = anomalyFile_["Param"].front();
	for (auto& anomaly : anomalyMap_)
	{
		// 異変の重みの設定
		const std::string& name = ANOMALY_LIST.at(anomaly.first);
		anomalyWeightMap_.emplace(anomaly.first, paramFile.at("Weights").at(name).get<int>());
	}	

	// パラメーターの設定
	timeMin_ = paramFile.at("timeMin");			
	timeMax_ = paramFile.at("timeMax");
	firstTime_ = paramFile.at("firstTime");

	// タイマー
	timer_ = std::make_unique<Timer>(firstTime_);
}

void AnomalyManager::Init()
{
	// タイマー初期化
	timer_->InitCountUp();

	// 異変発生を許可
	isOccurrence_ = true;

	// 各種異変の初期化処理
	for (auto& anomaly : anomalyMap_)
	{
		anomaly.second->Init();
	}
}

void AnomalyManager::Update()
{
	// 異変の更新
	if (updateType_ != TYPE::MAX)
	{
		anomalyMap_[updateType_]->Update();
	}

	// 異変発生が許可されている場合
	if (isOccurrence_)
	{
		// 制限時間に達した場合
		if (timer_->CountUp())
		{
			// 異変ファイルが空の場合
			if (anomalyFile_.empty())
			{
				// 処理を飛ばす
				return;
			}

			// 異変発生
			OccurAnomaly(TYPE::GRASS_ROOM);
			//OccurAnomaly(GetRandType());

			// 次回までの時間をランダム設定
			timer_->SetGoalTime(timeMin_ + GetRand(timeMax_ - timeMin_));

			// タイマー初期化
			timer_->InitCountUp();
		}
	}
}

void AnomalyManager::DebugDraw()
{
	DrawFormatString(0, 400, 0xff0000, L"発生までの時間：%d", timer_->GetCount());
}

void AnomalyManager::OccurAnomaly(const TYPE type)
{
	// 異変発生
	anomalyMap_[type]->Occurrence();
}

const AnomalyManager::TYPE AnomalyManager::GetRandType()
{
	// 重みの合計
	int totalWeight = 0;

	// 全パターンの総数を取得
	for (const auto& pair : anomalyWeightMap_)
	{
		if (pair.second > 0)
		{
			totalWeight += pair.second;
		}
	}

	// 全パターンが終了した場合
	if (totalWeight <= 0) 
	{
		return TYPE::MAX;
	}

	// 乱数を取得
	int rand = GetRand(totalWeight - 1);

	// 累積和
	int index = 0;
	for (auto& pair : anomalyWeightMap_)
	{
		// 0より大きい場合
		if (pair.second > 0)
		{
			// パターン数を加算
			index += pair.second;

			// 累積和の範囲内の場合
			if (rand < index)
			{
				// 値を減少
				pair.second--;

				// 要素を返す
				return pair.first;
			}
		}
	}
}

AnomalyManager::AnomalyManager()
{
	firstTime_ = 0.0f;
	timeMin_ = 0.0f;
	timeMax_ = 0.0f;
	isOccurrence_ = false;
	updateType_ = TYPE::MAX;
}

AnomalyManager::~AnomalyManager()
{
}