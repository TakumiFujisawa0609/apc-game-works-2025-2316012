#pragma once
#include <string>
#include <map>

class SoundType
{
public:

	// ƒTƒEƒ“ƒh‚ÌŽí—Þ
	enum class TYPE
	{
		NONE,
		BGM,
		SE,
	};

	// BGM‚ÌŽí—Þ
	enum class BGM
	{
		TITLE,
		GAME,
		RESULT,
		MAX
	};

	// SE‚ÌŽí—Þ
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
		ENEMY_FOUND,
		PLAYER_DIE,
		TV_NOISE_SNOW,
		MAX
	};

	// BGM‚ÌŽí—Þ‚ðŠÇ—
	static const std::map<std::string, BGM> KEY_BGM_MAP;

	// SE‚ÌŽí—Þ‚ðŠÇ—
	static const std::map<std::string, SE> KEY_SE_MAP;
};

