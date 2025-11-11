#pragma once
#include "ControllerDrawBase.h"

class ControllerDrawGhost : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルID</param>
	ControllerDrawGhost(const int model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawGhost() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// バッファーの更新
	void UpdateBuffer() override;
};

