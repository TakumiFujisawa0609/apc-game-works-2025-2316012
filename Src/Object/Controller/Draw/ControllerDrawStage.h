#pragma once
#include "ControllerDrawBase.h"

class ControllerDrawStage : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelId">モデルID</param>
	ControllerDrawStage(const int modelId);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawStage() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// バッファーの更新
	void UpdateBuffer() override;
};

