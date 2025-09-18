#pragma once
#include <nlohmann/json.hpp>
#include "../ActorBase.h"
#include "../../Common/Quaternion.h"

// JSON名前空間
using Json = nlohmann::json;

class CharacterBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーターデータ</param>
	CharacterBase(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase() = default;
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// トランスフォームを返す
	/// </summary>
	/// <returns>トランスフォーム</returns>
	const Transform& GetTransform() const { return transform_; }

protected:	

	// スピード
	const float SPEED_MOVE;

	// ダッシュスピード
	const float SPEED_RUN;

	// 重力
	const float GRAVITY;

	// 回転完了までの時間
	const float TIME_ROT;

	//デフォルトのアニメーション速度
	const float ANIM_DEFAULT_SPEED;

	//初期位置
	const VECTOR INITIAL_POS;

	//移動量
	VECTOR movePower_;

	//移動方向
	VECTOR moveDir_;	
	
	//回転ステップ時間
	float stepRotTime_;

	//現在の回転角度Y
	Quaternion rotY_;

	//目標回転
	Quaternion goalQuaRot_;

	/// <summary>
	/// 更新処理の適用
	/// </summary>
	virtual void UpdateApply() override;

	/// <summary>
	/// メインの描画処理
	/// </summary>
	virtual void DrawMain() override;

	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	virtual void InitAnimation() = 0;

	/// <summary>
	/// 回転の目標値を設定
	/// </summary>
	/// <param name="rotRad">回転角度</param>
	void SetGoalRotate(double rotRad);

	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name=""></param>
	void Rotate(void);
};

