#pragma once
#include "CoreTitleBase.h"

class Timer;

class TitleLogo : public CoreTitleBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleLogo();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleLogo();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// ロゴハンドル
	int logo_;

	// 描画位置
	Vector2 pos_;

};