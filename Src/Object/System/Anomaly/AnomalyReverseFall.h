#pragma once
#include "AnomalyBase.h"

class ControllerCameraJumpScare;
class ScreenShake;
class Timer;

class AnomalyReverseFall : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyReverseFall(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyReverseFall() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

	// 状態
	enum class STATE
	{
		NONE,
		CAMERA_PULL,	// カメラを引く
		EARTH_QUAKE,	// 地震
		REVERSE_FALL,	// 反転落下
		REVERSE_UP,		// 反転上昇
		CAMRA_BACK,		// 演出前に戻す
		MADNESS_TIME,	// 狂気値を増加
	};

	// フォグの開始位置と終了位置
	static constexpr float FOG_START = 1000.0f;
	static constexpr float FOG_END = 2000.0f;

	// 狂気値を更新する時間
	const float MADNESS_UP_TIME;

	// カメラの引き時間
	const float CAMERA_PULL_TIME;

	// 後ろに下がる移動量
	const float CAMERA_BACK_POW;

	// 画面揺れ時間
	const float SCREEN_SHAKE_TIME;

	// 画面揺れ強さ
	const float SCREEN_SHAKE_POW;

	// 下げる移動量
	const float CAMERA_DOWN_POW;

	// カメラを下げる時間
	const float CAMERA_DOWN_TIME;

	// オブジェクトのトランスフォーム情報
	const Json OBJ_TRANSFORM;

	// 異変を起こすオブジェクトのキー
	const std::string KEY_MESH = "CherryBlossomsWorldMesh";
	const std::string KEY_MAIN = "CherryBlossomsWorld";

	// カメラの回転角度
	static constexpr float CAMERA_ROTATION_DEG = 180.0f;

	// 状態
	STATE state_;

	// カメラ位置のバックアップ
	VECTOR preCameraPos_;

	// 注視点のバックアップ
	VECTOR preTargetPos_;

	// プレイヤー位置のバックアップ
	VECTOR prePlayerPos_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// カメラ制御
	std::unique_ptr<ControllerCameraJumpScare> camera_;

	// 画面シェイク
	std::unique_ptr<ScreenShake> screenShake_;

	// 状態変更関数管理
	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	// 更新処理
	std::function<void()> update_;

	// 各種状態別更新処理
	void UpdateNone() {};
	void UpdateCameraPull();
	void UpdateEarthQuake();
	void UpdateReverseFall();
	void UpdateReverseUp();
	void UpdateCameraBack();
	void UpdateMadnessTime();

	// 各種状態別変更処理
	void ChangeState(const STATE state);
	void ChangeStateNone();
	void ChangeStateCameraPull();
	void ChangeStateEarthQuake();
	void ChangeStateReverseFall();
	void ChangeStateReverseUp();
	void ChangeStateCameraBack();
	void ChangeStaetMadnessTime();

	// ステージの生成
	void CreateStage();

	// 演出終了後の処理
	void AfterDirection();
};