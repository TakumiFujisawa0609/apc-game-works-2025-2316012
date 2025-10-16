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
	~ControllerRotate() override;

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

	// 自身の角度Y
	Quaternion ownerQuaRot_;
};

