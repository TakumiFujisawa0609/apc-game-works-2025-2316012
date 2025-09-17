#pragma once
#include "ParametersLoadBase.h"

class ParametersLoadCharacter : public ParametersLoadBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParametersLoadCharacter();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParametersLoadCharacter() override = default;
	 
	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

};

