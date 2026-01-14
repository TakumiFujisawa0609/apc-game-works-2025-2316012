#pragma once
#include <functional>
#include <unordered_map>
#include "ControllerActionBase.h"

class Enemy;
class CharacterManager;
class ResourceManager;
class GameSystemManager;
class GameEffectManager;

class ControllerActionEnemyRespown : public ControllerActionBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		START,					// 開始
		WALK,					// 歩く
		MOVE_CAMERA_FORWARD,	// カメラを前方向に移動
		ZOOM_OUT,				// カメラをズームアウト
		ROAR,					// 吠える
		END,					// 終了
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者の参照インスタンス</param>
	ControllerActionEnemyRespown(Enemy& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerActionEnemyRespown() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;

private:

	// 所有者の参照インスタンス
	Enemy& owner_;

	// キャラクター管理クラス
	CharacterManager& charaMng_;

	// リソース管理クラス
	ResourceManager& resMng_;

	// ゲーム内のシステム管理クラス
	GameSystemManager& systemMng_;	

	// ゲームエフェクト管理クラス
	GameEffectManager& effectMng_;
	
	// 状態
	STATE state_;

	// 状態変更関数管理
	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	// 更新処理
	std::function<void()> update_;

	// 各種状態別遷移処理
	void ChangeState(const STATE state);
	void ChangeStateNone();
	void ChangeStateStart();
	void ChangeStateWalk();
	void ChangeStateMoveCameraForward();
	void ChangeStateZoomOut();
	void ChangeStateRoar();
	void ChangeStateEnd();

	// 各種状態別更新処理
	void UpdateNone() {};
	void UpdateStart();
	void UpdateWalk();
	void UpdateMoveCameraForward();
	void UpdateZoomOut();
	void UpdateRoar();
	void UpdateEnd();

};