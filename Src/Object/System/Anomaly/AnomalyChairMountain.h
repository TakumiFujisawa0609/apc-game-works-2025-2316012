#pragma once
#include "AnomalyBase.h"

class AnomalyChairMountain : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyChairMountain(const Json& param);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyChairMountain();
	
	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence() override;

private:

	// 初期化用パラメータ
	const Json INIT_PARAM;

	// オブジェクト指定キー
	const std::string KEY = "ChairMountain";

};