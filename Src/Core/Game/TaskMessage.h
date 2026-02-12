#pragma once
#include "../../Common/Image.h"
#include "CoreGameBase.h"

class Timer;

class TaskMessage : public CoreGameBase
{
public:

	enum class STATE
	{
		WAIT,		// 待機
		DELAY,		// 遅延
		ADMISSION,	// 入場
		DISPLAY,	// 表示
		EXIT,		// 退場
	};

	enum class TYPE
	{
		START,			// ゲーム開始時の説明
		NIGHT_VIEW,		// 暗視の説明
		MADNESS,		// 狂気ゲージの説明
		ANOMALY,		// 異変の説明
		ENEMY,			// 敵の説明
		FINAL_WARNING,	// 最終告知
		TIPS_GHOST,		// ゴーストのヒント
		TIPS_PAINT,		// ペイントのヒント
		TIPS_SOUND,		// サウンドのヒント
		MAX,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">説明</param>
	TaskMessage(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TaskMessage() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// メッセージの表示種類を変更
	/// </summary>
	/// <param name="type">表示種類</param>
	/// <param name="delayTime">遅れ時間</param>
	void ChangeMessageType(const TYPE type, const float delayTime = 0.0f);

private:

	// テキスト名リスト
	const std::vector<std::string> TEXT_NAME_LIST =
	{
		"textStart",
		"textNightView",
		"textMadness",
		"textAnomaly",
		"textEnemy",
		"textAnomaly",
		"textFinalWarning",
		"textPaintTips",
		"textSoundTips",
		"textGhostTips",
	};

	// 移動時間
	const float MOVE_TIME;

	// 表示時間
	const float DISPLAY_TIME;

	// ゴーストのY座標
	const int GHOST_POS_Y;

	// テキストのY座標
	const int TEXT_POS_Y;

	// 開始位置X座標
	const int START_POS_X;

	// 終了位置X座標
	const int END_POS_X;

	// ゴーストとテキストの位置調整
	const int GHOST_TEXT_OFFSET_X;
	
	//揺れの振幅
	const float GHOST_SHAKE_SPEED;		
	
	//揺れの速さ
	const float GHOST_SHAKE_AMPLITUDE;	

	// フォントサイズ
	const int FONT_SIZE;

	// フォント太さ
	const int FONT_THICK;

	// フォント名前
	const std::string FONT_NAME;

	// ゴースト画像
	Image imgGhost_;

	// 状態
	STATE state_;

	// 現在のメッセージタイプ
	TYPE type_;

	// ステップ
	float moveStep_;

	// ゴーストシェイク用ステップ
	float ghostShakeStep_;

	// 表示位置
	float startPosX_;
	float endPosX_;

	// メッセージ文字列
	CharacterString strMes_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// メッセージリスト管理マップ
	std::unordered_map<TYPE, std::wstring> messageListMap_;

	// 状態別更新処理管理
	std::unordered_map<STATE, std::function<void()>> stateChangeMap_;

	// 更新処理
	std::function<void()> updateFunc_;

	// 状態別更新処理
	void UpdateWait() {};	
	void UpdateDelayt();
	void UpdateAdmission();
	void UpdateDisplay();
	void UpdateExit();

	// 状態変更処理
	void ChangeState(const STATE state);
	void ChageeStateWait();
	void ChageeStateDelay();
	void ChangeStateAdmission();
	void ChangeStateDisplay();
	void ChangeStateExit();

	// 移動処理
	void Move();
};