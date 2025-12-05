#pragma once
#include "ControllerDrawBase.h"
class ControllerDrawCherryBlossomsWorld : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルID</param>
	ControllerDrawCherryBlossomsWorld(const int model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawCherryBlossomsWorld();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// 環境光
	const VECTOR CHERRY_AMBIENT = { 1.0f, 1.0f, 1.0f };
	
	// バッファーの更新
	void UpdateBuffer() override;
};

