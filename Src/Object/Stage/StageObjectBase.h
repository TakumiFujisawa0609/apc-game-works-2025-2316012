#pragma once
#include "../ActorBase.h"

class StageObjectBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageObjectBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageObjectBase() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() override;

protected:

	//処理の適用処理
	void UpdateApply() override;

	//メインの描画処理
	void DrawMain() override;
};

