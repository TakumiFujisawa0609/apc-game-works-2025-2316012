#pragma once
#include <functional>
#include <unordered_map>
#include "GameStateBase.h"

class Enemy;
class CharacterManager;
class ResourceManager;
class GameSystemManager;
class GameEffectManager;

class GameStateEnemyRespown : public GameStateBase
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
	GameStateEnemyRespown();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStateEnemyRespown() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 出現する敵のインスタンス
	Enemy* target_;

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