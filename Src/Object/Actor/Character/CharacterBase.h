#pragma once
#include <memory>
#include <string>
#include "../ActorBase.h"
#include "../../../Common/Quaternion.h"

class ControllerAnimation;
class ControllerActionBase;
class ControllerMove;
class ControllerRotate;
class ControllerGravity;
class ControllerOnHitBase;

class CharacterBase : public ActorBase
{
public:

	// アニメ種別キー
	static const std::string ANIM_IDLE;	// 待機
	static const std::string ANIM_WALK;	// 歩く
	static const std::string ANIM_RUN;	// 走る

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
	/// 読み込み処理
	/// </summary>
	virtual void Load() override;
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 更新後の処理
	/// </summary>
	virtual void PostUpdate() override;

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のコライダー</param>
	virtual void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider) override;

	/// <summary>
	/// 移動速度量を返す
	/// </summary>
	/// <returns>移動速度量</returns>
	const float GetSpeedMove() const { return SPEED_MOVE; }

	/// <summary>
	/// ダッシュ速度量を返す
	/// </summary>
	/// <returns>ダッシュ速度量</returns>
	const float GetSpeedRun() const { return SPEED_RUN; }

	/// <summary>
	/// 重力を返す
	/// </summary>
	/// <returns>重力</returns>
	const float GetGravity() const { return GRAVITY; }

	/// <summary>
	/// 回転完了までの時間を返す
	/// </summary>
	/// <returns>回転完了までの時間</returns>
	const float GetTimeRot() const { return TIME_ROT; }

	/// <summary>
	/// 現在の移動速度を返す
	/// </summary>
	/// <returns>現在の移動速度</returns>
	const float GetMoveSpeed() const { return moveSpeed_; }	
	
	/// <summary>
	/// コライダーカプセルの半径を返す
	/// </summary>
	/// <returns></returns>
	const float GetCapsuleRadius() const;

	/// <summary>
	/// 回転角度を返す
	/// </summary>
	/// <returns>角度Y</returns>
	const double GetRotDeg() const { return rotDeg_; }

	/// <summary>
	/// 現在の移動方向を返す
	/// </summary>
	/// <returns>現在の移動方向</returns>
	const VECTOR GetMoveDir() const { return moveDir_; }

	/// <summary>
	/// 現在のジャンプ力を返す
	/// </summary>
	/// <returns>現在のジャンプ量</returns>
	const VECTOR GetJumpPow() const { return jumpPow_; }

	/// <summary>
	/// 移動後の座標を返す
	/// </summary>
	/// <returns>移動後の座標</returns>
	const VECTOR GetMovedPos() const { return movedPos_; }

	/// <summary>
	/// カプセル座標の上部分を返す
	/// </summary>
	/// <returns>カプセル座標の上</returns>
	const VECTOR GetCapsuleTopPos() const;

	/// <summary>
	/// カプセル座標の下部分を返す
	/// </summary>
	/// <returns>カプセル座標の下</returns>
	const VECTOR GetCapsuleDownPos() const;

	/// <summary>
	/// アニメーション制御クラスを返す
	/// </summary>
	/// <returns>アニメーション制御クラス</returns>
	ControllerAnimation& GetControllerAnimation() const { return *animation_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const VECTOR& pos) { transform_.pos = pos; }

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot">角度</param>
	void SetRot(const Quaternion& rot) { transform_.quaRot = rot; }

	/// <summary>
	/// 回転角度の設定
	/// </summary>
	/// <param name="rotDeg">回転角度</param>
	void SetRotDeg(const double rotDeg) { rotDeg_ = rotDeg; }

	/// <summary>
	/// 移動速度の設定
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }

	/// <summary>
	/// 移動方向の設定
	/// </summary>
	/// <param name="moveDir">移動方向</param>
	void SetMoveDir(const VECTOR& moveDir) { moveDir_ = moveDir; }

	/// <summary>
	/// ジャン力の設定の設定
	/// </summary>
	/// <param name="jumpPow">ジャンプ力</param>
	void SetJumpPow(const VECTOR& jumpPow) { jumpPow_ = jumpPow; }
	
	/// <summary>
	/// 移動後の座標を設定
	/// </summary>
	/// <param name="movedPos"></param>
	void SetMovedPos(const VECTOR& movedPos) { movedPos_ = movedPos; }

protected:	
	
	// スピード
	const float SPEED_MOVE;

	// ダッシュスピード
	const float SPEED_RUN;

	// 重力
	const float GRAVITY;

	// 回転完了までの時間
	const float TIME_ROT;

	// デフォルトのアニメーション速度
	const float ANIM_DEFAULT_SPEED;

	// 回転角度(DEG)
	double rotDeg_;

	// 移動速度
	float moveSpeed_;

	// 移動方向
	VECTOR moveDir_;

	// ジャンプ力
	VECTOR jumpPow_;

	// 移動後の座標
	VECTOR movedPos_;

	// アニメーション制御クラス
	std::unique_ptr<ControllerAnimation> animation_;

	// アクション制御クラス
	std::unique_ptr<ControllerActionBase> action_;

	// 移動制御クラス
	std::unique_ptr<ControllerMove> move_;

	// 回転制御クラス
	std::unique_ptr<ControllerRotate> rotate_;

	// 重力制御クラス
	std::unique_ptr<ControllerGravity> gravity_;

	// 衝突後処理の制御クラス
	std::unique_ptr<ControllerOnHitBase> onHit_;

	/// <summary>
	/// メインの描画処理
	/// </summary>
	virtual void DrawMain() override;

	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	virtual void InitAnimation() = 0;

	/// <summary>
	/// デバッグ時の描画
	/// </summary>
	void DebugDraw() override;
};