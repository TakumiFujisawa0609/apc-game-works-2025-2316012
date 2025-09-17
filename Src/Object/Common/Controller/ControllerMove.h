#pragma once

class CharacterBase;

class ControllerMove
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerMove(CharacterBase& owner);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerMove() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	
private:

	//所有者
	CharacterBase& owner_;

	// 移動速度
	float moveSpeed_;
	
	// ダッシュ速度
	float dashSpeed_;
	
	// 重力
	float gravity_;
	
	// 回転完了までの時間
	float timeRot_;
};

