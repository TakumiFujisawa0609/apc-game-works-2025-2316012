#include "EffectType.h"

// •¶Žš—ñ‚©‚çenum ‚Ì‘Î‰ž•\
const std::map<std::string, EffectType::TYPE> EffectType::KEY_MAP =
{
	{"petalFall",EffectType::TYPE::PETAL_FALL},
	{"extinction",EffectType::TYPE::EXTINCTION},
	{"orb",EffectType::TYPE::ORB},
	{"roar",EffectType::TYPE::ROAR}
};