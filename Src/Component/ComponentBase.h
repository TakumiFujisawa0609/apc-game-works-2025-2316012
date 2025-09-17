#pragma once
#include <memory>

class ActorBase;

class ComponentBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ComponentBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ComponentBase() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {};

protected:

};

