#pragma once
#include "ParameterLoadBase.h"

class ParameterLoadStage : public ParameterLoadBase
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParameterLoadStage();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParameterLoadStage() override = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;
};

