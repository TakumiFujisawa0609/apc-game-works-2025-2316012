#include <random>
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../Actor/Stage/Painting.h"
#include "AnomalyPainting.h"

namespace
{
	// 乱数生成エンジン
	std::random_device rd;
	std::mt19937 gen(rd());
}

AnomalyPainting::AnomalyPainting()
{
}

AnomalyPainting::~AnomalyPainting()
{
}

void AnomalyPainting::Load()
{
	// 異変用テクスチャの取得
	anomalyPaintingTexture1_ = resMng_.GetHandle("anomalyPainting1");
}

void AnomalyPainting::Init()
{
	// 絵画の番号リストを格納
	painting1Size_ = static_cast<int>(stageMng_.GetStageObjects("Painting01").size());
	for (int i = 0; i < painting1Size_; i++)
	{
		paintingList_.push_back(i);
	}
	int painting2Size = static_cast<int>(stageMng_.GetStageObjects("Painting02").size());
	for (int i = 0; i < painting2Size; i++)
	{
		paintingList_.push_back(painting1Size_ + i);
	}
}

void AnomalyPainting::Occurrence(Json& param)
{
	// 配列の範囲から乱数を生成
	std::uniform_int_distribution<std::size_t> dist(0, paintingList_.size() - 1);

	// ランダムな値を決定
	int randIndex = dist(gen);

	// 絵の番号を決定
	int cnt = paintingList_[randIndex];

	// 選ばれた要素は削除
	paintingList_.erase(paintingList_.begin() + randIndex);

	// 絵のポインタ
	Painting* painting;

	// 番号が絵1の番号の場合
	if (cnt < painting1Size_)
	{
		painting = dynamic_cast<Painting*>(stageMng_.GetStageObjects("Painting01")[cnt].get());
	}
	// 絵2の場合
	else
	{	
		cnt -= painting1Size_;
		painting = dynamic_cast<Painting*>(stageMng_.GetStageObjects("Painting02")[cnt].get());
	}

	// テクスチャ切り替え
	painting->ChangeTexture(anomalyPaintingTexture1_, 0);

	// 異変状態にする
	painting->SetIsActive(true);
}
