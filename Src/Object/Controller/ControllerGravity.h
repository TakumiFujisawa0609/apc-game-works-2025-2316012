#pragma once
#include <DxLib.h>
#include "ControllerBase.h"

class CharacterBase;

class ControllerGravity : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	ControllerGravity(CharacterBase& owner);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerGravity() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

	// 所有者のインスタンス
	CharacterBase& owner_;
	
	// 重力
	const float GRAVITY;

	// 重力による移動量
	void CalcGravityPow();
};