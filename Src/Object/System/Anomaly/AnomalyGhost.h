#pragma once
#include "AnomalyBase.h"

class AnomalyGhost : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメータ</param>
	AnomalyGhost(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyGhost();

	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence() override;

private:

	// パラメータファイル
	Json paramFile_;
};