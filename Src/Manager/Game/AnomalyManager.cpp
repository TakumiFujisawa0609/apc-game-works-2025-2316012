#include <DxLib.h>
#include "../../Object/System/Anomaly/AnomalyBase.h"
#include "../../Object/System/Anomaly/AnomalyAppearance.h"
#include "../../Object/System/Anomaly/AnomalyGhost.h"
#include "../../Object/System/Anomaly/AnomalyPainting.h"
#include "../../Object/System/Anomaly/AnomalyChairMountain.h"
#include "../../Object/System/Anomaly/AnomalyBloodyRoom.h"
#include "../../Object/System/Anomaly/AnomalyGrassRoom.h"
#include "../../Utility/UtilityLoad.h"
#include "../../Core/Common/Timer.h"
#include "AnomalyManager.h"

void AnomalyManager::Load()
{
	// 異変情報の格納
	anomalyFile_ = UtilityLoad::GetJsonMapArrayData(ANOMALY_FILE);

	// 出現異変処理
	anomalyMap_[TYPE::GHOST] = std::make_unique<AnomalyGhost>();
	anomalyMap_[TYPE::PAINTING] = std::make_unique<AnomalyPainting>();
	anomalyMap_[TYPE::CHAIR_MOUNTAIN] = std::make_unique<AnomalyChairMountain>();
	anomalyMap_[TYPE::BLOODY_ROOM] = std::make_unique<AnomalyBloodyRoom>();
	anomalyMap_[TYPE::GRASS_ROOM] = std::make_unique<AnomalyGrassRoom>();

	// 各種異変の読み込み処理
	auto& paramFile = anomalyFile_["Param"][0];
	for (auto& anomaly : anomalyMap_)
	{
		// リソース等の読み込み処理
		anomaly.second->Load();

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

	// 各種異変の読み込み処理
	for (auto& anomaly : anomalyMap_)
	{
		anomaly.second->Init();
	}

	OccurAnomaly(TYPE::BLOODY_ROOM);
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
		//OccurAnomaly(TYPE::BLOODY_ROOM);
		//OccurAnomaly(GetRandType());

		// 次回までの時間をランダム設定
		timer_->SetGoalTime(timeMin_ + GetRand(timeMax_ - timeMin_));

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
	const std::string typeString = ANOMALY_LIST.at(type);

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
}

AnomalyManager::~AnomalyManager()
{
}