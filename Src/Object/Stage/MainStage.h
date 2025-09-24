#pragma once
#include "StageObjectBase.h"

class ColliderModel;

class MainStage : public StageObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MainStage();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MainStage() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	//メインの描画処理
	void DrawMain() override;
};