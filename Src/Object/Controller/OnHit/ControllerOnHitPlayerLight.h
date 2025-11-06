#pragma once
#include "ControllerOnHitBase.h"

class Player;
class ControllerLight;

class ControllerOnHitPlayerLight : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	ControllerOnHitPlayerLight(Player& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerOnHitPlayerLight();

private:

	// 所有者のインスタンス
	Player& owner_;

	// プレイヤーライトの参照
	ControllerLight& light_;

	// 衝突物別処理
	void OnHitMainStage(const std::weak_ptr<ColliderBase>& opponentCollider);			// メインステージ
};

