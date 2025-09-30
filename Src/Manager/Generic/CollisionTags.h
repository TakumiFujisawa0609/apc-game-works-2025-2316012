#pragma once
#include <string>
#include <map>

class CollisionTags
{
public:

	// 衝突物の種類
	enum class TAG
	{
		PLAYER,
		MAIN_STAGE,
		MAX
	};

	// タグの数
	static constexpr int TAG_COUNT = static_cast<int>(TAG::MAX);

	// 各種タグの名前を管理するマップ
	static const std::map<TAG, std::string> TAG_NAME_MAP;
};