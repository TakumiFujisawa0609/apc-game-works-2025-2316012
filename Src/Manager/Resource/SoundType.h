#pragma once
#include <string>
#include <map>

class SoundType
{
public:

	// ƒTƒEƒ“ƒh‚Ìí—Ş
	enum class TYPE
	{
		NONE,
		BGM,
		SE,
	};

	// BGM‚Ìí—Ş
	enum class BGM
	{
		TITLE,
		GAME,
		RESULT,
		MAX
	};

	// SE‚Ìí—Ş
	enum class SE
	{
		BREATHING,
		FOOT_STEP,
		GAME_START,
		GIRLS_LAUGHTER,
		GIRLS_HELP,
		HEART_BEAT,
		JUMP_SCARE,
		KEBOARD,
		NOISE_GATAN,
		NOISE_GON,
		NOISE_METAL,
		NOISE_PACHI,
		NOISE_SWITCH,
		REPORT,
		REPORTING,
		CAMERA_MODE_CAHNGE,
		MAX
	};

	// BGM‚Ìí—Ş‚ğŠÇ—
	static const std::map<std::string, BGM> KEY_BGM_MAP;

	// SE‚Ìí—Ş‚ğŠÇ—
	static const std::map<std::string, SE> KEY_SE_MAP;
};

