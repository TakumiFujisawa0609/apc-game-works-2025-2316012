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

private:	

	// バッファーサイズ
	static constexpr int BUFFER_SIZE = 1;
	
	// ボタン位置
	static constexpr int POS_X = 384;
	static constexpr int POS_Y = 500;

	// ボタンサイズ
	static constexpr int SIZE_X = 512;
	static constexpr int SIZE_Y = 384;
};