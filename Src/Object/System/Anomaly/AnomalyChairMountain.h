#pragma once
#include "AnomalyBase.h"

class AnomalyChairMountain : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyChairMountain();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyChairMountain();
	
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

	const std::string KEY = "chairMountain";

};

