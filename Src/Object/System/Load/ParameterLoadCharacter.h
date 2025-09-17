#pragma once
#include "ParameterLoadBase.h"

class ParameterLoadCharacter : public ParameterLoadBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParameterLoadCharacter();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParameterLoadCharacter() override = default;
	 
	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

};

