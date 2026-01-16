#pragma once
#include <DxLib.h>
#include <memory>
#include <functional>
#include <unordered_map>
#include "GameStateBase.h"

class Enemy;
class Timer;
class ScreenShake;
class ControllerCameraBase;
class SceneManager;
class SoundManager;

class GameStateEnemyRespown : public GameStateBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		WAIT,
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

	// 初期カメラ情報
	static constexpr VECTOR START_CAMERA_POS = { -1340.0f, 52.0f, -620.0f };
	static constexpr VECTOR START_CAMERA_TARGET = { -1430.0f, 30.0f, -802.0f };

	// 寄せのカメラ情報
	static constexpr VECTOR ZOOM_IN_CAMERA_POS = { -1255.0f, 170.0f, -696.0f };
	static constexpr VECTOR ZOOM_IN_CAMERA_TARGET = { -1450.0f, 125.0f, -686.0f };

	// 最終カメラ情報
	static constexpr VECTOR END_CAMERA_POS = { -1080.0f, 113.0f, -705.0f };
	static constexpr VECTOR END_CAMERA_TARGET = { -1280.0f, 95.0f, -694.0f };

	// カメラ移動時間
	static constexpr float CAMERA_MOVE_TIME = 2.0f;

	// ズームアウト時間
	static constexpr float ZOOM_OUT_TIME = 1.3f;

	// 歩く時間
	static constexpr float WALK_TIME = 2.0f;

	// 咆哮時間
	static constexpr float ROAR_TIME = 5.0f;

	// 画面揺れ時間
	static constexpr float SCREEN_SHAKE_TIME = 4.0f;

	// 画面揺れ強さ
	static constexpr float SCREEN_SHAKE_POWER = 8.0f;

	// 敵の移動速度
	static constexpr float ENEMY_MOVE_SPEED = 0.5f;

	// 遷移用エフェクトの速度倍率
	static constexpr float TRANSITION_EFFECT_SPEED_RATE = 0.7f;

	// 出現する敵のインスタンス
	Enemy* target_;

	// シーン管理クラス
	SceneManager& scnMng_;

	// サウンド管理クラス
	SoundManager& sndMng_;

	// 状態
	STATE state_;

	int oldTexture_;

	// 炎のトランジション用ステップ
	float fireStep_;

	// 状態変更関数管理
	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	// 更新処理
	std::function<void()> update_;

	// 時間
	std::unique_ptr<Timer> timer_;

	// 画面シェイク
	std::unique_ptr<ScreenShake> screenShake_;

	// カメラ制御
	std::unique_ptr<ControllerCameraBase> cameraController_;

	// 各種状態別遷移処理
	void ChangeState(const STATE state);
	void ChangeStateWait();
	void ChangeStateStart();
	void ChangeStateWalk();
	void ChangeStateMoveCameraForward();
	void ChangeStateZoomOut();
	void ChangeStateRoar();
	void ChangeStateEnd();

	// 各種状態別更新処理
	void UpdateWait();
	void UpdateStart();
	void UpdateWalk();
	void UpdateMoveCameraForward();
	void UpdateZoomOut();
	void UpdateRoar();
	void UpdateEnd();
};