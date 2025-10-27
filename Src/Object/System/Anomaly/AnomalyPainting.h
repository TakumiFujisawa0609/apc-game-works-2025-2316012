#pragma once
#include <vector>
#include "AnomalyBase.h"

class Painting;

class AnomalyPainting : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyPainting();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyPainting();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence(Json& param) override;

private:

	// 絵の種類数
	static constexpr int TYPE = 2;

	// 異変用の絵
	int anomalyPaintingTexture1_;

	// 絵1の要素数
	int painting1Size_;

	// 絵の番号リスト
	std::vector<int> paintingList_;
};

