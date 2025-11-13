#pragma once
#include "ControllerDrawBase.h"

class ControllerDrawPainting : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルID</param>
	ControllerDrawPainting(const int model);
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawPainting() override;
	 
	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// バッファーの更新
	void UpdateBuffer() override;
};

