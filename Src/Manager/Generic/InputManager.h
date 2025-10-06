#pragma once
#include <map>
#include <functional>
#include <vector>
#include "../../Template/Singleton.h"
#include "../../Common/Vector2.h"
#include "Input.h"

class InputManager : public Singleton<InputManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<InputManager>;

public:

	/// <summary>
	/// 入力状況の種類
	/// </summary>
	enum class TYPE
	{
		NONE,

		PLAYER_MOVE_RIGHT,
		PLAYER_MOVE_LEFT,
		PLAYER_MOVE_UP,
		PLAYER_MOVE_DOWN,
		PLAYER_JUMP,
		PLAYER_DASH,

		SELECT_RIGHT,
		SELECT_LEFT,
		SELECT_DOWN,
		SELECT_UP,
		SELECT_DECISION,
		SELECT_CANCEL,

		PAUSE,

		DEBUG_SCENE_CHANGE,
		DEBUG_CAMERA_CHANGE,
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// リソースの破棄
	/// </summary>
	void Release();

	/// <summary>
	/// 指定した状況での各入力機器の押下判定
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合入力中,falseの場合未入力</returns>
	bool IsNew(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::KEY_PAD1);

	/// <summary>
	/// 指定した状況での各入力機器の押下判定(最初のみ)
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合入力中,falseの場合未入力</returns>
	bool IsTrgDown(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::KEY_PAD1);

	/// <summary>
	/// 指定した状況での各入力機器の離した判定
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合離したタイミング,falseの場合まだ押下中</returns>
	bool IsTrgUp(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::KEY_PAD1);

	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <returns>マウス座標の取得</returns>
	Vector2 GetMousePos() const;

private:

	// 入力トリガーの情報
	struct TriggerInfo
	{
		int key = -1;
		Input::JOYPAD_BTN padButton = Input::JOYPAD_BTN::MAX;
		Input::JOYPAD_STICK padStick = Input::JOYPAD_STICK::MAX;
	};

	// 入力判定クラス
	std::unique_ptr<Input> input_;

	// 入力状況別のトリガーを管理するマップ
	std::map<TYPE, TriggerInfo> triggerMap_;

	// 入力状況に応じた押下処理を管理するマップ
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcNewMap_;
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcTrgDownMap_;
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcTrgUpMap_;

	// 入力状況に応じたトリガーを登録
	void RegisterTrigger(
		const TYPE type, 
		const int key = -1, 
		const Input::JOYPAD_BTN padButton = Input::JOYPAD_BTN::MAX,
		const Input::JOYPAD_STICK padStick = Input::JOYPAD_STICK::MAX);

	// 処理の登録
	void RegisterTriggerFunction(
		const InputManager::TYPE type, 
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs,
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgDownFuncs,
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgUpFuncs);

	// キー関係の入力判定
	bool IsNewKey(const TYPE type);
	bool IsTrgDownKey(const TYPE type);
	bool IsTrgUpKey(const TYPE type);

	// パッドボタンの入力判定
	bool IsNewPadButton(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgDownPadButton(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgUpPadButton(const TYPE type, const Input::JOYPAD_NO padNo);

	// パッドスティックの入力判定
	bool IsNewPadStick(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgDownPadStick(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgUpPadStick(const TYPE type, const Input::JOYPAD_NO padNo);

	// コンストラクタ
	InputManager();

	// デストラクタ
	~InputManager();
};