#include "../../Manager/Generic/InputManager.h"
#include "InputPlayer.h"

InputPlayer::InputPlayer() :
	input_(InputManager::GetInstance())
{
	RegisterCheckKey(CONFIG::RIGHT, std::bind(&InputPlayer::CheckKeyRight, this));
	RegisterCheckKey(CONFIG::LEFT, std::bind(&InputPlayer::CheckKeyLeft, this));
	RegisterCheckKey(CONFIG::FORWARD, std::bind(&InputPlayer::CheckKeyForward, this));
	RegisterCheckKey(CONFIG::BACK, std::bind(&InputPlayer::CheckKeyBack, this));
	RegisterCheckKey(CONFIG::DASH, std::bind(&InputPlayer::CheckKeyDash, this));
	RegisterCheckKey(CONFIG::JUMP, std::bind(&InputPlayer::CheckKeyJump, this));
}

bool InputPlayer::CheckKey(const CONFIG config)
{
	return keyCheckMap_[config]();
}

void InputPlayer::RegisterCheckKey(const CONFIG config, std::function<bool()> func)
{
	keyCheckMap_.emplace(config, func);
}

bool InputPlayer::CheckKeyRight() const
{
	return input_.IsNew(KEY_INPUT_D) || input_.IsStickNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_STICK::L_STICK_RIGHT);
}

bool InputPlayer::CheckKeyLeft() const
{
	return input_.IsNew(KEY_INPUT_A) || input_.IsStickNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_STICK::L_STICK_LEFT);
}

bool InputPlayer::CheckKeyForward() const
{
	return input_.IsNew(KEY_INPUT_W) || input_.IsStickNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_STICK::L_STICK_UP);
}

bool InputPlayer::CheckKeyBack() const
{
	return input_.IsNew(KEY_INPUT_S) || input_.IsStickNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_STICK::L_STICK_DOWN);
}

bool InputPlayer::CheckKeyDash() const
{
	return input_.IsNew(KEY_INPUT_LSHIFT) || input_.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BUTTON);
}

bool InputPlayer::CheckKeyJump() const
{
	return input_.IsNew(KEY_INPUT_SPACE) || input_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RB_RIGHT);
}
