#pragma once
#include "ControllerBase.h"

class CharacterBase;

class ControllerMove : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerMove(CharacterBase& owner);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerMove() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void) override;
	
private:

	// 所有者
	CharacterBase& owner_;
};

