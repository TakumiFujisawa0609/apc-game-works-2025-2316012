#pragma once
#include "ControllerBase.h"
#include "../../Common/Quaternion.h"

class CharacterBase;

class ControllerRotate : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	ControllerRotate(CharacterBase& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerRotate() override = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

	// 所有者の参照
	CharacterBase& owner_;

	// 回転時間
	const float TIME_ROT;	
	
	//回転用時時間
	float stepRotTime_;

	//目標までの回転情報
	Quaternion goalQuaRot_;

	// 自身の角度Y
	Quaternion myRotY_;

	/// <summary>
	/// 目的角度まで回転処理
	/// </summary>
	void SetGoalRotate();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// 回転情報の適用
	/// </summary>
	void Apply();
};

