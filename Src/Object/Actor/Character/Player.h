#pragma once
#include <memory>
#include <functional>
#include "CharacterBase.h"

class Player : public CharacterBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		ALIVE,
		DEAD,
	};

	//アニメ種別キー
	static const std::string ANIM_JUMP;		//ジャンプ
	static const std::string ANIM_DIE;		//死ぬ
	static const std::string ANIM_SLEEP;	//眠る

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター</param>
	Player(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// ジャンプ量を返す
	/// </summary>
	/// <returns>ジャンプ量</returns>
	const float GetJumpAmount() const { return JUMP_AMOUNT; }

	/// <summary>
	/// ジャンプ受付時間を返す
	/// </summary>
	/// <returns>ジャンプ受付時間</returns>
	const float GetJumpAcceptTime() const { return JUMP_ACCEPT_TIME; }

	/// <summary>
	/// ジャンプ用ステップを返す
	/// </summary>
	/// <returns>ジャンプ用ステップ</returns>
	const float GetStepJump() const { return stepJump_; }

	/// <summary>
	/// ジャンプ判定を返す
	/// </summary>
	/// <returns>ジャンプ判定</returns>
	const bool IsJump() const { return isJump_; }

	/// <summary>
	/// ジャンプ判定を設定
	/// </summary>
	/// <param name="isJump">ジャンプ判定</param>
	void SetIsJump(const bool isJump) { isJump_ = isJump; }

	/// <summary>
	/// ジャンプ用ステップを設定
	/// </summary>
	/// <param name="step">ジャンプ用ステップ</param>
	void SetStepJump(const float step) { stepJump_ = step; }

private:

	// ジャンプ量
	const float JUMP_AMOUNT;

	// ジャンプ受付時間
	const float JUMP_ACCEPT_TIME;

	// ジャンプアニメーション
	const float ANIM_JUMP_SPEED;

	// ジャンプ判定
	bool isJump_;

	// ジャンプ用ステップ
	float stepJump_;

	// 状態
	STATE state_;

	// 状態別の更新関数マップ
	std::unordered_map<STATE, std::function<void()>> stateUpdateFuncMap_;

	// メインの更新処理
	void UpdateBody() override;	

	// アニメーションの初期化
	void InitAnimation() override;

	// トランスフォームの初期化
	void InitTransform() override;
	
	// 状態別更新処理の登録
	void RegisterStateUpdateFunc(const STATE state, std::function<void()> update);

	// 状態別更新処理	
	void UpdateNone() {};	// 何もしない
	void UpdateAlive();		// 生存状態の更新
	void UpdateDead();		// 死亡状態の更新

	// デバッグ描画
	void DebugDraw() override;
};