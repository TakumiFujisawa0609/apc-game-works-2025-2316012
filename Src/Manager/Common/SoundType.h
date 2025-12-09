#pragma once
#include <string>
#include <map>

class SoundType
{
public:

	// ÉTÉEÉìÉhÇÃéÌóﬁ
	enum class TYPE
	{
		NONE,
		BGM,
		SE,
	};

	// BGMÇÃéÌóﬁ
	enum class BGM
	{
		TITLE_MAIN,
		TITLE_EXPLANATION,
		TITLE_SELECT,
		GAME,
		RESULT,
		MAX
	};

	// SEÇÃéÌóﬁ
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
		BUG_1,
		BUG_2,
		SELECT,
		DECISION,
		CAVEAT,
		DRUM_ROLL,
		DRUM_ROLL_END,
		RESULT_START,
		GLASS_BREAK,
		GRASS_LAND,
		EARTHQUAKE,
		ANOMALY_CAMERA_ROLL,
		MAX
	};

	// BGMÇÃéÌóﬁÇä«óù
	static const std::map<std::string, BGM> KEY_BGM_MAP;

	// SEÇÃéÌóﬁÇä«óù
	static const std::map<std::string, SE> KEY_SE_MAP;
};

