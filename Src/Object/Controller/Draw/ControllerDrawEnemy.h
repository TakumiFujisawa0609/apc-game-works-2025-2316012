#pragma once
#include "ControllerDrawBase.h"

class ControllerDrawEnemy : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルID</param>
	ControllerDrawEnemy(const int model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawEnemy() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// バッファーの更新
	void UpdateBuffer() override;
};

