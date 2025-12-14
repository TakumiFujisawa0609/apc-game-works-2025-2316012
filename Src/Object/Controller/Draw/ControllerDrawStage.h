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
	// バッファーサイズ
	static constexpr int BUFFER_PS_SIZE = 4;
	static constexpr int BUFFER_VS_SIZE = 2;

	// バッファーの更新
	void UpdateBuffer() override;
};

