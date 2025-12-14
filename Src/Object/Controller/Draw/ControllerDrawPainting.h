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

	// バッファーサイズ
	static constexpr int BUFFER_PS_SIZE = 3;
	static constexpr int BUFFER_VS_SIZE = 2;

	// バッファーの更新
	void UpdateBuffer() override;
};

