#pragma once
#include "AnomalyBase.h"

class AnomalyAppearance :  public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyAppearance();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyAppearance() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence(Json& param) override;

private:
};

