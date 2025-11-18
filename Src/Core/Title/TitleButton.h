#pragma once
#include "CoreTitleBase.h"

class TitleButton : public CoreTitleBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleButton();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleButton() override;

	/// <summary>
	/// 初期化
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

