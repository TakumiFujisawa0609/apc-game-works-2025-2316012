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
	~ControllerMove() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理(座標を更新)
	/// </summary>
	/// <param name=""></param>
	void Update() override;

	/// <summary>
	/// 移動後の座標を返す
	/// </summary>
	/// <returns>移動後の座標</returns>
	const VECTOR GetMovedPos();
	
private:

	// 所有者
	CharacterBase& owner_;
};

