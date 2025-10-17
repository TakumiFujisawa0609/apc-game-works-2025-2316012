#pragma once
#include "CharacterBase.h"

class ControllerPathFinder;
class ControllerActionEnemy;
class ControllerOnHitEnemyView;

class Enemy : public CharacterBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		ALIVE,	// 生存
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

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のコライダー</param>
	void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider) override;

	/// <summary>
	/// 初期位置用のインデックスを返す
	/// </summary>
	/// <returns>初期位置用のインデックス</returns>
	const int GetFirstPosIndex() { return FIRST_POS_INDEX; }

	/// <summary>
	/// 視野角の角度を返す
	/// </summary>
	/// <returns>視野角の角度</returns>
	const float GetViewAngle() { return VIEW_ANGLE; }

	/// <summary>
	/// 視野角の範囲を返す
	/// </summary>
	/// <returns>視野角の範囲</returns>
	const float GetViewRange() { return VIEW_RANGE; }

	/// <summary>
	/// 経路探索処理の参照を返す
	/// </summary>
	/// <returns>経路探索処理</returns>
	ControllerPathFinder& GetPathFinder() { return *pathFinder_; }

	/// <summary>
	/// アクションクラスの参照を返す
	/// </summary>
	ControllerActionEnemy* GetActionEnemy();

	/// <summary>
	/// 移動位置リストの参照を返す
	/// </summary>
	/// <returns>移動位置リスト</returns>
	std::vector<VECTOR>& GetMovePosList() { return movePosList_; }

private:

	// 初期位置用のインデックス
	const int FIRST_POS_INDEX;

	// 視野角の角度
	const float VIEW_ANGLE;

	// 視野角の範囲
	const float VIEW_RANGE;

	// 状態
	STATE state_;

	// 移動位置リスト
	std::vector<VECTOR> movePosList_;

	// 経路探索処理
	std::unique_ptr<ControllerPathFinder> pathFinder_;

	// 敵の視野用衝突後処理
	std::unique_ptr<ControllerOnHitEnemyView> onHitView_;

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
	void UpdateAlive();		// 生存

	// デバッグ描画
	void DebugDraw() override;
};