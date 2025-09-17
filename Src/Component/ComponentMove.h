#pragma once
#include "ComponentBase.h"

class Transform;

class ComponentMove : public ComponentBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownerTransform">所有者のトランスフォーム</param>
	ComponentMove(Transform& ownerTransform);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ComponentMove() override = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

	//所有者のトランスフォーム
	Transform& ownerTransform_;
};

