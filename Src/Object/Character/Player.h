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
	/// アニメーション種類
	/// </summary>
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		DEAD,
		SLEEP,
		MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parameter">パラメーター</param>
	Player(ParameterPlayer& parameter);

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

	//状態
	STATE state_;

	//パラメーター管理クラスの参照
	ParameterPlayer& parameter_;

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
	void UpdateNone();
	void UpdateAlive();
	void UpdateDead();
};