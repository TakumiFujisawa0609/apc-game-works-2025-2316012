#pragma once
#include "ControllerOnHitBase.h"

class Player;
class ActorBase;

class ControllerOnHitPlayer : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	ControllerOnHitPlayer(Player& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerOnHitPlayer();

private:

	// 衝突物別処理
	void OnHitStage(const ActorBase& actor);

};

