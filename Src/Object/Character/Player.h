#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include "CharacterBase.h"

class InputPlayer;
class ParameterPlayer;
class ControllerAnimation;

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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター</param>
	Player(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() override = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// ジャンプ力
	const float POW_JUMP;

	// ジャンプ受付時間
	const float JUMP_ACCEPT_TIME;

	// ジャンプアニメーション
	const float ANIM_JUMP_SPEED;

	//アニメ種別キー
	const std::string ANIM_JUMP = "jump";	//ジャンプ
	const std::string ANIM_DIE = "die";		//死ぬ
	const std::string ANIM_SLEEP = "sleep";	//眠る

	//ジャンプ判定
	bool isJump_;

	//状態
	STATE state_;

	//入力管理クラス
	std::unique_ptr<InputPlayer> inputPlayer_;

	//アニメーション制御クラス
	std::unique_ptr<ControllerAnimation> animation_;

	//状態別の更新関数マップ
	std::unordered_map<STATE, std::function<void()>> stateUpdateFuncMap_;

	//メインの更新処理
	void UpdateMain() override;	

	//更新処理の適用
	void UpdateApply() override;

	//アニメーションの初期化
	void InitAnimation() override;
	
	//状態別更新処理の登録
	void RegisterStateUpdateFunc(const STATE state, std::function<void()> update);

	//状態別更新処理	
	void UpdateNone() {};	// 何もしない
	void UpdateAlive();		// 生存状態の更新
	void UpdateDead();		// 死亡状態の更新

	//移動操作
	void ProcessMove();

	//ジャンプ操作
	void ProcessJump();

	//着地モーションの終了
	bool IsEndLanding();

	//デバッグ描画
	void DebugDraw() override;
};