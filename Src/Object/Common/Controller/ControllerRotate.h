#pragma once
#include "ControllerBase.h"
#include "../../../Common/Quaternion.h"

class ControllerRotate : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerRotate(const float timeRot);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerRotate() override;

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// 目的角度まで回転処理
	/// </summary>
	/// <param name="rotRad">回転角度</param>
	void SetGoalRotate(double rotRad);

	/// <summary>
	/// 回転情報の設定
	/// </summary>
	/// <param name="targetQuaternion">設定したいターゲットの回転情報</param>
	void SetRotate(Quaternion& targetQuaternion);

private:

	//回転時間
	const float TIME_ROT;

	//Yの回転情報
	Quaternion rotY_;

	//目標までの回転情報
	Quaternion goalQuaRot_;

	//回転用時時間
	float stepRotTime_;
};

