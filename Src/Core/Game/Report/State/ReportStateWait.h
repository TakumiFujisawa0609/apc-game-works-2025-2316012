#pragma once
#include "ReportStateBase.h"

class ReportStateWait : public ReportStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親インスタンス</param>
	ReportStateWait(ReportSystem& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ReportStateWait() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

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

private:
};