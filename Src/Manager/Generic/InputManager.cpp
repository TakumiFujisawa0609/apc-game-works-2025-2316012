#include <DxLib.h>
#include "Input.h"
#include "InputManager.h"

void InputManager::Init()
{
	// 入力判定処理の生成と初期化
	input_ = std::make_unique<Input>();
	input_->Init();	

	// 入力状況に応じたトリガーの登録
	using BTN = Input::JOYPAD_BTN;
	using STICK = Input::JOYPAD_STICK;

	// プレイヤー操作
 	RegisterTrigger(TYPE::PLAYER_MOVE_RIGHT, KEY_INPUT_D, BTN::MAX, STICK::L_STICK_RIGHT);
	RegisterTrigger(TYPE::PLAYER_MOVE_LEFT, KEY_INPUT_A, BTN::MAX, STICK::L_STICK_LEFT);
	RegisterTrigger(TYPE::PLAYER_MOVE_UP, KEY_INPUT_W, BTN::MAX, STICK::L_STICK_UP);
	RegisterTrigger(TYPE::PLAYER_MOVE_DOWN, KEY_INPUT_S, BTN::MAX, STICK::L_STICK_DOWN);
	RegisterTrigger(TYPE::PLAYER_JUMP, KEY_INPUT_SPACE, BTN::RB_DOWN);
	RegisterTrigger(TYPE::PLAYER_DASH, KEY_INPUT_LSHIFT, BTN::RB_RIGHT);

	// メニュー操作
	RegisterTrigger(TYPE::SELECT_RIGHT, KEY_INPUT_D, BTN::MAX, STICK::L_STICK_RIGHT);
	RegisterTrigger(TYPE::SELECT_LEFT, KEY_INPUT_A, BTN::MAX, STICK::L_STICK_LEFT);
	RegisterTrigger(TYPE::SELECT_UP, KEY_INPUT_W, BTN::MAX, STICK::L_STICK_UP);
	RegisterTrigger(TYPE::SELECT_DOWN, KEY_INPUT_S, BTN::MAX, STICK::L_STICK_DOWN);
	RegisterTrigger(TYPE::SELECT_DECISION, KEY_INPUT_SPACE, BTN::RB_RIGHT);
	RegisterTrigger(TYPE::SELECT_CANCEL, KEY_INPUT_BACK, BTN::RB_DOWN);
	RegisterTrigger(TYPE::PAUSE, KEY_INPUT_BACK, BTN::SELECT);

	// 報告処理
	RegisterTrigger(TYPE::ANOMARY_REPORT, KEY_INPUT_R, BTN::RB_LEFT);

	// デバッグ操作
	RegisterTrigger(TYPE::DEBUG_SCENE_CHANGE, KEY_INPUT_RSHIFT);
	RegisterTrigger(TYPE::DEBUG_CAMERA_CHANGE, KEY_INPUT_TAB);
	RegisterTrigger(TYPE::CREATE_POSITION, KEY_INPUT_C);
}

void InputManager::Update()
{
	input_->Update();
}

void InputManager::Release()
{
	triggerMap_.clear();
	funcNewMap_.clear();
	funcTrgDownMap_.clear();
	funcTrgUpMap_.clear();
	input_->Release();
}

bool InputManager::IsNew(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto& func : funcNewMap_[type])
	{
		if (func(type, padNo))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgDown(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto& func : funcTrgDownMap_[type])
	{
		if (func(type, padNo))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgUp(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto& func : funcTrgUpMap_[type])
	{
		if (func(type, padNo))
		{
			return true;
		}
	}
	return false;
}

Vector2 InputManager::GetMousePos() const
{
	return input_->GetMousePos();
}

void InputManager::RegisterTrigger(const TYPE type, const int key, const Input::JOYPAD_BTN padButton, const Input::JOYPAD_STICK padStick)
{
	// トリガーの情報を設定
	TriggerInfo info = { key, padButton, padStick };

	// 情報の格納
	triggerMap_[type] = info;

	// 各種処理の配列
	std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs;
	std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> isTrgDownFuncs;
	std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> isTrgUpFuncs;

	// キーが登録されている場合
	if (info.key != -1)
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO) { return IsNewKey(t); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO) { return IsTrgDownKey(t); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO) { return IsTrgUpKey(t); });
	}
	// パッドのボタンが登録されている場合
	if (info.padButton != Input::JOYPAD_BTN::MAX)
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsNewPadButton(t, padNo); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgDownPadButton(t, padNo); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgUpPadButton(t, padNo); });
	}
	// パッドのスティックが登録されている場合
	if (info.padStick != Input::JOYPAD_STICK::MAX)
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsNewPadStick(t, padNo); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgDownPadStick(t, padNo); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgUpPadStick(t, padNo); });
	}

	// 処理の登録
	RegisterTriggerFunction(type, newFuncs, isTrgDownFuncs, isTrgUpFuncs);
}

void InputManager::RegisterTriggerFunction(const TYPE type, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgDownFuncs, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgUpFuncs)
{
	funcNewMap_.emplace(type, newFuncs);
	funcTrgDownMap_.emplace(type, trgDownFuncs);
	funcTrgUpMap_.emplace(type, trgUpFuncs);
}

bool InputManager::IsNewKey(const TYPE type)
{
	return input_->IsNew(triggerMap_[type].key);
}

bool InputManager::IsTrgDownKey(const TYPE type)
{
	return input_->IsTrgDown(triggerMap_[type].key);
}

bool InputManager::IsTrgUpKey(const TYPE type)
{
	return input_->IsTrgUp(triggerMap_[type].key);
}

bool InputManager::IsNewPadButton(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsPadBtnNew(padNo, triggerMap_[type].padButton);
}

bool InputManager::IsTrgDownPadButton(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsPadBtnTrgDown(padNo, triggerMap_[type].padButton);
}

bool InputManager::IsTrgUpPadButton(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsPadBtnTrgUp(padNo, triggerMap_[type].padButton);
}

bool InputManager::IsNewPadStick(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsStickNew(padNo, triggerMap_[type].padStick);
}

bool InputManager::IsTrgDownPadStick(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsStickDown(padNo, triggerMap_[type].padStick);
}

bool InputManager::IsTrgUpPadStick(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsStickUp(padNo, triggerMap_[type].padStick);
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}
