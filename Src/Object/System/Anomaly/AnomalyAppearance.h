#pragma once
#include "AnomalyBase.h"

class AnomalyAppearance :  public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyAppearance(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyAppearance() override;

	/// <summary>
	/// 発生処理
	/// </summary>
	void Occurrence() override;

private:
};

