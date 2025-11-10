#pragma once
#include <memory>
#include "../../../Common/Quaternion.h"
#include "../ControllerBase.h"

class CharacterBase;
class CollisionManager;
class SoundManager;

class ControllerActionBase : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerActionBase(CharacterBase& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerActionBase() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override = 0;

	/// <summary>
	/// デバッグ表示
	/// </summary>
	virtual void DebugDraw() {};
	
protected:
	
	// 移動方向別回転角度(DEG)
	static constexpr double ROT_DEG_RIGHT = 90.0f;	//右
	static constexpr double ROT_DEG_LEFT = 270.0f;	//左
	static constexpr double ROT_DEG_FRONT = 0.0f;	//前
	static constexpr double ROT_DEG_BACK = 180.0f;	//後ろ		
	
	// 衝突判定管理クラスの参照
	CollisionManager& collMng_;
	SoundManager& sndMng_;

private:

	// 所有者の参照
	CharacterBase& owner_;
};

