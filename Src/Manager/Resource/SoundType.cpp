#include "SoundType.h"

// •¶Žš—ñ‚©‚çenum ‚Ì‘Î‰ž•\
const std::map<std::string, SoundType::BGM> SoundType::KEY_BGM_MAP =
{
	{"bgmTitle",SoundType::BGM::TITLE},
	{"bgmGame", SoundType::BGM::GAME},
	{"bgmResult", SoundType::BGM::RESULT}
};

const std::map<std::string, SoundType::SE> SoundType::KEY_SE_MAP =
{
    {"seBreathing", SoundType::SE::BREATHING},
    {"seFootStep", SoundType::SE::FOOT_STEP},
    {"seGameStart", SoundType::SE::GAME_START},
    {"seGirlsLaughter", SoundType::SE::GIRLS_LAUGHTER},
    {"seGirlsHelp", SoundType::SE::GIRLS_HELP},
    {"seHeartBeat", SoundType::SE::HEART_BEAT},
    {"seJumpScare", SoundType::SE::JUMP_SCARE},
    {"seKeboard", SoundType::SE::KEBOARD},
    {"seNoiseGatan", SoundType::SE::NOISE_GATAN},
    {"seNoiseGon", SoundType::SE::NOISE_GON},
    {"seNoiseMetal", SoundType::SE::NOISE_METAL},
    {"seNoisePachi", SoundType::SE::NOISE_PACHI},
    {"seNoiseSwitch", SoundType::SE::NOISE_SWITCH},
    {"seReport", SoundType::SE::REPORT},
    {"seReporting", SoundType::SE::REPORTING},
    {"seCameraChangeMode", SoundType::SE::CAMERA_MODE_CAHNGE},
	{"seEnemyFound", SoundType::SE::ENEMY_FOUND},
	{"sePlayerDie", SoundType::SE::PLAYER_DIE}
};