#pragma once
#include "../Title/TitleStateBase.h"

class TitleStateSelect : public TitleStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleStateSelect(SceneTitle& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleStateSelect() override;

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

