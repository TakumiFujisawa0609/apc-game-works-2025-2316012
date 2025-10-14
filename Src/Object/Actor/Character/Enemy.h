#pragma once
#include "CharacterBase.h"

class Enemy : public CharacterBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		SEARCH,	// 探す
		CHASE,	// 追う
		ACTION,	// 攻撃
	};

	//アニメ種別キー
	static const std::string ANIM_ACTION;		//攻撃

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター</param>
	Enemy(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// 状態
	STATE state_;

	// 移動位置リスト
	std::vector<VECTOR> movePosList_;

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
	void UpdateSearch();	// 詮索状態の更新
	void UpdateChase();		// 追跡状態の更新
	void UpdateAction();	// 攻撃状態の更新

	// デバッグ描画
	void DebugDraw() override;
};

